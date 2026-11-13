#include "SceneRenderer.hpp"
#include <Graphitech.hpp>
#include <Graphitech/ECS/ECS.hpp>
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <Graphitech/Engine/Engine.hpp>
#include <Window/Window.hpp>
#include <Debugger/Debug.hpp>
#include <Render/RenderContext.hpp>
#include <Render/SpriteRenderSystem.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

MOD("SceneRenderer");

namespace Graphitech {

struct SceneRenderer::Impl {
    GLFWwindow *window = nullptr;
    int width = 640;
    int height = 480;
    int logicalWidth = 1280;
    int logicalHeight = 720;
    Graphitech::EventHandler* handler = nullptr;
    bool printUiToConsole = false;

    // Viewport info for mouse coordinate conversion
    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = 640;
    int viewportHeight = 480;
    
    // Camera
    float cameraOffsetX = 0.0f;
    float cameraOffsetY = 0.0f;

    // Game render passes
    Graphitech::RenderHook backgroundHook;
    Graphitech::RenderHook overlayHook;
    
    // Sprite rendering system
    std::unique_ptr<Graphitech::SpriteRenderSystem> spriteRenderSystem = nullptr;
    bool spriteRenderSystemInitialized = false;
};

SceneRenderer::SceneRenderer() : impl_(std::make_unique<Impl>()) {}
SceneRenderer::~SceneRenderer() { terminate(); }

void SceneRenderer::setLogicalSize(int logicalW, int logicalH)
{
    impl_->logicalWidth = logicalW;
    impl_->logicalHeight = logicalH;
}

void SceneRenderer::setCameraOffset(float x, float y)
{
    impl_->cameraOffsetX = x;
    impl_->cameraOffsetY = y;
}

void SceneRenderer::setBackgroundHook(RenderHook hook)
{
    impl_->backgroundHook = std::move(hook);
}

void SceneRenderer::setOverlayHook(RenderHook hook)
{
    impl_->overlayHook = std::move(hook);
}

bool SceneRenderer::init(int w, int h, [[maybe_unused]] const char *title)
{
    impl_->width = w; impl_->height = h;

    if(!glfwInit()){
        std::cerr << "glfwInit failed\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    impl_->window = nullptr;
    return true;
}

void SceneRenderer::terminate()
{
    if (impl_->spriteRenderSystem) {
        impl_->spriteRenderSystem->Release();
        impl_->spriteRenderSystem.reset();
    }
    
    if(impl_->window){
        glfwDestroyWindow(impl_->window);
        impl_->window = nullptr;
        glfwTerminate();
    }
    
    // Destroy RenderContext
    RenderContext::Destroy();
}

void SceneRenderer::pollEvents()
{
    glfwPollEvents();
}

bool SceneRenderer::shouldClose() const
{
    return impl_->window ? glfwWindowShouldClose(impl_->window) : true;
}

void SceneRenderer::setup2D(int windowW, int windowH, int logicalW, int logicalH, IRenderAPI& api, SceneRenderer::Impl* impl)
{
    // Logical viewport size (what the game world uses)
    float logicalWidth = (float)logicalW;
    float logicalHeight = (float)logicalH;
    
    // Calculate aspect ratios
    float windowAspect = (float)windowW / (float)windowH;
    float logicalAspect = logicalWidth / logicalHeight;
    
    // Calculate viewport to preserve aspect ratio (letterboxing/pillarboxing)
    int vpX, vpY, vpW, vpH;
    if (windowAspect > logicalAspect) {
        // Window is wider - pillarbox (black bars on sides)
        vpH = windowH;
        vpW = (int)(windowH * logicalAspect);
        vpX = (windowW - vpW) / 2;
        vpY = 0;
    } else {
        // Window is taller - letterbox (black bars on top/bottom)
        vpW = windowW;
        vpH = (int)(windowW / logicalAspect);
        vpX = 0;
        vpY = (windowH - vpH) / 2;
    }
    
    // Store viewport info for mouse coordinate conversion
    if (impl) {
        impl->viewportX = vpX;
        impl->viewportY = vpY;
        impl->viewportWidth = vpW;
        impl->viewportHeight = vpH;
    }
    
    api.SetViewport(vpX, vpY, vpW, vpH);
    
    float offX = impl ? impl->cameraOffsetX : 0.0f;
    float offY = impl ? impl->cameraOffsetY : 0.0f;
    
    // Apply camera offset (shake) by shifting the ortho window
    api.SetOrtho2D(-offX, logicalWidth - offX, logicalHeight - offY, -offY, -1.0f, 1.0f);
}

void SceneRenderer::render(ECS::Scene &scene)
{
    // Lazy initialization of RenderContext once we have a valid OpenGL context
    static bool renderContextInitialized = false;
    
    GLFWwindow* cur = glfwGetCurrentContext();
    if(!cur){
        if (Graphitech::Engine::_window && Graphitech::Engine::_window->GetHandle()) {
            glfwMakeContextCurrent(Graphitech::Engine::_window->GetHandle());
            cur = glfwGetCurrentContext();
        }
    }
    if(!cur) {
        DEBUG_WARN("SceneRenderer::render called but no GL context is current, skipping frame");
        return;
    }
    
    // Initialize RenderContext now that we have a valid GL context
    if (!renderContextInitialized) {
        if (!RenderContext::Create(Graphitech::RenderAPIType::OpenGL)) {
            DEBUG_WARN("Failed to create RenderContext");
            return;
        }
        auto& api = RenderContext::GetAPI();
        if (!api.Initialize()) {
            DEBUG_WARN("Failed to initialize RenderAPI");
            return;
        }
        renderContextInitialized = true;
        DEBUG_INFO("RenderContext initialized successfully");
    }
    
    // Initialize SpriteRenderSystem lazily
    if (!impl_->spriteRenderSystemInitialized) {
        impl_->spriteRenderSystem = std::make_unique<Graphitech::SpriteRenderSystem>();
        if (!impl_->spriteRenderSystem->Initialize()) {
            DEBUG_WARN("Failed to initialize SpriteRenderSystem");
            impl_->spriteRenderSystem.reset();
        } else {
            impl_->spriteRenderSystem->UpdateProjection(impl_->logicalWidth, impl_->logicalHeight);
            impl_->spriteRenderSystemInitialized = true;
            DEBUG_INFO("SpriteRenderSystem initialized successfully");
        }
    }
    
    auto& api = RenderContext::GetAPI();
    
    int fbW, fbH; 
    glfwGetFramebufferSize(cur, &fbW, &fbH);
    int w = fbW, h = fbH;
    setup2D(w, h, impl_->logicalWidth, impl_->logicalHeight, api, impl_.get());
    
    // Games may override the clear colour through map values (clearColorR/G/B)
    Color clearColor{0.1f, 0.12f, 0.15f, 1.0f};
    {
        const std::string curMap = scene.getCurrentMap();
        auto *cr = scene.getMapValue<float>(curMap, "clearColorR");
        auto *cg = scene.getMapValue<float>(curMap, "clearColorG");
        auto *cb = scene.getMapValue<float>(curMap, "clearColorB");
        if (cr && cg && cb) clearColor = Color{*cr, *cg, *cb, 1.0f};
    }
    api.SetClearColor(clearColor);
    // Clear the whole framebuffer, letterbox bars included
    api.SetViewport(0, 0, w, h);
    api.Clear(true, true);

    RenderHookInfo hookInfo;
    hookInfo.framebufferWidth = w;
    hookInfo.framebufferHeight = h;
    hookInfo.viewportX = impl_->viewportX;
    hookInfo.viewportY = impl_->viewportY;
    hookInfo.viewportWidth = impl_->viewportWidth;
    hookInfo.viewportHeight = impl_->viewportHeight;
    hookInfo.logicalWidth = impl_->logicalWidth;
    hookInfo.logicalHeight = impl_->logicalHeight;
    hookInfo.cameraOffsetX = impl_->cameraOffsetX;
    hookInfo.cameraOffsetY = impl_->cameraOffsetY;

    if (impl_->backgroundHook) {
        impl_->backgroundHook(hookInfo);
    }
    api.SetViewport(impl_->viewportX, impl_->viewportY, impl_->viewportWidth, impl_->viewportHeight);
    
    static int frameCounter = 0;
    ++frameCounter;
    if ((frameCounter % 60) == 0) {
        std::ostringstream oss; oss << "SceneRenderer::render - ctx=" << (void*)cur << " fb="<< fbW<<","<< fbH <<" clear=(0.1,0.12,0.15)"; DEBUG_INFO(oss.str());
    }
    
    std::string err = api.GetLastError();
    if (!err.empty()) {
        std::ostringstream oss; oss << "Renderer: API Error after clear: " << err; DEBUG_WARN(oss.str());
    }

    auto lines = scene.getEntitiesWith<ECS::RenderLine>();
    for (auto e : lines) {
        auto *ln = scene.getComponent<ECS::RenderLine>(e);
        if (!ln) continue;
        api.SetDrawColor(Color{ln->r, ln->g, ln->b, 1.0f});
        api.SetLineWidth(ln->width);
        api.BeginDraw(Graphitech::PrimitiveType::Lines);
        api.Vertex(ln->x1, ln->y1);
        api.Vertex(ln->x2, ln->y2);
        api.EndDraw();
    }

    auto circles = scene.getEntitiesWith<ECS::RenderCircle, ECS::PositionComponent>();
    for (auto e : circles) {
        auto *p = scene.getComponent<ECS::PositionComponent>(e);
        auto *rc = scene.getComponent<ECS::RenderCircle>(e);
        if (!p || !rc) continue;
        float cx = p->x; float cy = p->y;
        api.SetDrawColor(Color{rc->r, rc->g, rc->b, 1.0f});
        const int seg = 24;
        if (rc->filled) {
            api.BeginDraw(Graphitech::PrimitiveType::TriangleFan);
            api.Vertex(cx, cy);
            for (int i = 0; i <= seg; ++i) {
                float a = (float)i / (float)seg * 2.0f * 3.1415926f;
                api.Vertex(cx + cosf(a) * rc->radius, cy + sinf(a) * rc->radius);
            }
            api.EndDraw();
        } else {
            api.BeginDraw(Graphitech::PrimitiveType::LineStrip);
            for (int i = 0; i <= seg; ++i) {
                float a = (float)i / (float)seg * 2.0f * 3.1415926f;
                api.Vertex(cx + cosf(a) * rc->radius, cy + sinf(a) * rc->radius);
            }
            api.EndDraw();
        }
    }

    auto crosses = scene.getEntitiesWith<ECS::RenderCross, ECS::PositionComponent>();
    for (auto e : crosses) {
        auto *p = scene.getComponent<ECS::PositionComponent>(e);
        auto *xr = scene.getComponent<ECS::RenderCross>(e);
        if (!p || !xr) continue;
        float cx = p->x; float cy = p->y;
        api.SetDrawColor(Color{xr->r, xr->g, xr->b, 1.0f});
        api.SetLineWidth(xr->width);
        api.BeginDraw(Graphitech::PrimitiveType::Lines);
        api.Vertex(cx - xr->half, cy - xr->half); api.Vertex(cx + xr->half, cy + xr->half);
        api.Vertex(cx - xr->half, cy + xr->half); api.Vertex(cx + xr->half, cy - xr->half);
        api.EndDraw();
    }

    // Render all sprites with textures using batch rendering
    if (impl_->spriteRenderSystem && impl_->spriteRenderSystemInitialized) {
        impl_->spriteRenderSystem->SetCameraOffset(impl_->cameraOffsetX, impl_->cameraOffsetY);
        impl_->spriteRenderSystem->RenderSprites(scene, impl_->logicalWidth, impl_->logicalHeight);
    }

    if (impl_->overlayHook) {
        api.SetViewport(impl_->viewportX, impl_->viewportY, impl_->viewportWidth, impl_->viewportHeight);
        impl_->overlayHook(hookInfo);
    }

    const std::string curMap = scene.getCurrentMap();
    if(!curMap.empty()){
        auto *ui = scene.getMapValue<std::vector<ECS::UIItem>>(curMap, "ui");
        if(ui){
            int idx = 0;
            for(auto &it : *ui){
                int x = it.x ? it.x : (w - 200);
                int y = it.y ? it.y : (10 + idx * 20);
                float r = it.r, g = it.g, b = it.b;
                api.SetDrawColor({r, g, b, 1.0f});
                api.BeginDraw(Graphitech::PrimitiveType::TriangleFan);
                api.Vertex((float)x, (float)y);
                api.Vertex((float)(x+8), (float)y);
                api.Vertex((float)(x+8), (float)(y+8));
                api.Vertex((float)x, (float)(y+8));
                api.EndDraw();
                if (impl_->printUiToConsole) {
                    std::cout << it.text << "\n";
                }
                ++idx;
            }
        }
    }

    // glColor3f(1.0f, 0.0f, 0.0f);
    // glBegin(GL_QUADS);
    // glVertex2f(2.0f, 2.0f); glVertex2f(12.0f, 2.0f);
    // glVertex2f(12.0f, 12.0f); glVertex2f(2.0f, 12.0f);
    // glEnd();
}

void SceneRenderer::setEventHandler(Graphitech::EventHandler* h){
    (void)h;
}

void SceneRenderer::setPrintUIToConsole(bool v) {
    impl_->printUiToConsole = v;
}

int SceneRenderer::getViewportWidth() const {
    return impl_->viewportWidth;
}

int SceneRenderer::getViewportHeight() const {
    return impl_->viewportHeight;
}

glm::vec2 SceneRenderer::screenToWorldCoords(int screenX, int screenY) const {
    // Convert screen pixel coordinates to world coordinates
    // Considering the viewport offset and scaling due to letterboxing/pillarboxing
    
    int vpX = impl_->viewportX;
    int vpY = impl_->viewportY;
    int vpW = impl_->viewportWidth;
    int vpH = impl_->viewportHeight;
    
    // Normalize to viewport [0, 1]
    float normX = (float)(screenX - vpX) / (float)vpW;
    float normY = (float)(screenY - vpY) / (float)vpH;
    
    // Clamp to [0, 1]
    normX = std::max(0.0f, std::min(1.0f, normX));
    normY = std::max(0.0f, std::min(1.0f, normY));
    
    // Convert to world coordinates
    // glOrtho already inverts Y, so we don't need to invert again
    float worldX = normX * impl_->logicalWidth;
    float worldY = normY * impl_->logicalHeight;
    
    return glm::vec2(worldX, worldY);
}

} // namespace Graphitech
