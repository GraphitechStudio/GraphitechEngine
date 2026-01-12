#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

/**
 * @file UIRenderer.hpp
 * @brief OpenGL-based renderer for UI widgets
 */


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <string>
#include <map>
#include <vector>

namespace Graphitech {
namespace UI {

/**
 * @brief Color structure
 */
struct Color {
    float r, g, b, a;
    
    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    
    static Color White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
    static Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
    static Color Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
    static Color Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
    static Color Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
    static Color Yellow() { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
    static Color Gray(float v = 0.5f) { return Color(v, v, v, 1.0f); }
};

/**
 * @brief Low-level OpenGL renderer for UI primitives
 */
class UIRenderer {
public:
    UIRenderer();
    ~UIRenderer();

    bool Initialize(int windowWidth, int windowHeight);
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void SetWindowSize(int width, int height);
    
    // Drawing primitives
    void DrawRect(float x, float y, float width, float height, const Color& color, bool filled = true);
    void DrawRoundedRect(float x, float y, float width, float height, float radius, const Color& color, bool filled = true);
    void DrawCircle(float x, float y, float radius, const Color& color, bool filled = true);
    void DrawLine(float x1, float y1, float x2, float y2, float thickness, const Color& color);
    void DrawText(const std::string& text, float x, float y, float size, const Color& color);
    void DrawTexture(unsigned int textureId, float x, float y, float width, float height, const Color& tint = Color::White());
    
    // Scissor test for clipping
    void PushScissor(float x, float y, float width, float height);
    void PopScissor();

private:
    void CreateShaders();
    void CreateBuffers();
    
    GLuint shaderProgram_;
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    
    int windowWidth_;
    int windowHeight_;
    glm::mat4 projection_;
    
    struct ScissorRect {
        int x, y, width, height;
    };
    std::vector<ScissorRect> scissorStack_;
};

} // namespace UI
} // namespace Graphitech

#endif // UIRENDERER_HPP
