/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** SceneRenderer.hpp - Generic ECS scene renderer
*/

#ifndef SCENERENDERER_HPP
#define SCENERENDERER_HPP

#include <functional>
#include <memory>
#include <glm/vec2.hpp>

namespace ECS { class Scene; }
namespace Graphitech { struct EventHandler; struct IRenderAPI; }

namespace Graphitech {

    /**
     * @brief Generic renderer for ECS scenes.
     *
     * @details Handles rendering of all entities in an ECS scene using
     * the graphics HAL (IRenderAPI). Supports:
     * - 2D orthographic projection
     * - Logical viewport with aspect ratio preservation
     * - Component-based rendering (RenderCircle, RenderLine, etc.)
     * - Screen-to-world coordinate conversion
     *
     * This renderer is game-agnostic and can be used by any application.
     *
     * @ingroup Rendering
     */
    /**
     * @brief Frame information handed to render hooks.
     *
     * The framebuffer covers the whole window; the viewport is the letterboxed
     * area that shows the logical game world.
     */
    struct RenderHookInfo {
        int framebufferWidth = 0;
        int framebufferHeight = 0;
        int viewportX = 0;
        int viewportY = 0;
        int viewportWidth = 0;
        int viewportHeight = 0;
        int logicalWidth = 0;
        int logicalHeight = 0;
        float cameraOffsetX = 0.0f;
        float cameraOffsetY = 0.0f;
    };

    /// Callback used by games to draw custom passes with their own GL state.
    using RenderHook = std::function<void(const RenderHookInfo&)>;

    class SceneRenderer {
    public:
        SceneRenderer();
        ~SceneRenderer();

        // Disable copy
        SceneRenderer(const SceneRenderer&) = delete;
        SceneRenderer& operator=(const SceneRenderer&) = delete;

        /**
         * @brief Initializes the renderer (optional, for standalone usage).
         * @param w Window width in pixels
         * @param h Window height in pixels
         * @param title Window title (unused if window already exists)
         * @return true if initialization succeeded
         */
        bool init(int w = 640, int h = 480, const char *title = "GraphitechEngine");
        
        /**
         * @brief Sets the logical viewport size for game world coordinates.
         * @param logicalW Logical width (e.g., 1280)
         * @param logicalH Logical height (e.g., 720)
         */
        void setLogicalSize(int logicalW, int logicalH);

        /**
         * @brief Terminates the renderer and cleans up resources.
         */
        void terminate();

        /**
         * @brief Polls input events (if managing window).
         */
        void pollEvents();

        /**
         * @brief Checks if the window should close.
         * @return true if close was requested
         */
        bool shouldClose() const;

        /**
         * @brief Renders all entities in the given scene.
         * @param scene The ECS scene to render
         */
        void render(ECS::Scene &scene);

        /**
         * @brief Enables/disables UI text printing to console (debug).
         * @param v true to enable console printing
         */
        void setPrintUIToConsole(bool v);

        /**
         * @brief Sets an event handler for input events (deprecated).
         * @param h Pointer to event handler
         */
        void setEventHandler(Graphitech::EventHandler* h);

        /**
         * @brief Sets the camera offset (e.g. for screen shake).
         * @param x Offset X in logical pixels
         * @param y Offset Y in logical pixels
         */
        void setCameraOffset(float x, float y);

        /**
         * @brief Sets a pass drawn right after the clear, before primitives and
         * sprites. The viewport covers the whole framebuffer during the call
         * (letterbox bars included) and is restored afterwards.
         */
        void setBackgroundHook(RenderHook hook);

        /**
         * @brief Sets a pass drawn after the sprites (effects, glows), with the
         * game viewport active.
         */
        void setOverlayHook(RenderHook hook);

        /**
         * @brief Converts screen coordinates to logical game world coordinates.
         * @param screenX Screen X coordinate in pixels
         * @param screenY Screen Y coordinate in pixels
         * @return Logical world coordinates
         */
        glm::vec2 screenToWorldCoords(int screenX, int screenY) const;

        /**
         * @brief Gets the framebuffer/viewport width.
         * @return Viewport width in pixels
         */
        int getViewportWidth() const;

        /**
         * @brief Gets the framebuffer/viewport height.
         * @return Viewport height in pixels
         */
        int getViewportHeight() const;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;

        static void setup2D(int windowW, int windowH, int logicalW, int logicalH, 
                           IRenderAPI& api, Impl* impl);
    };

} // namespace Graphitech

#endif // SCENERENDERER_HPP
