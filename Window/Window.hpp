#ifndef WINDOW_HPP
    #define WINDOW_HPP
#include <Graphitech.hpp>

#include "WindowEvent.hpp"

namespace Graphitech {

    /**
     * @brief Configuration structure for window creation.
     *
     * @details Contains the initial parameters used to create a window,
     * including dimensions and title.
     */
    struct WindowDescriptor final {
        /** @brief Window width in pixels. */
        uint32_t width = 1280u;
        /** @brief Window height in pixels. */
        uint32_t height = 720u;
        /** @brief Window title displayed in the title bar. */
        std::string title = "Graphitech Engine";
    };

    /**
     * @brief Main window management class.
     *
     * @details The Window class handles the creation, updating, and destruction
     * of application windows. It provides an interface to GLFW window operations
     * and integrates with the engine's event system for window-related events.
     *
     * @ingroup Window
     */
    class Window final {
        public:
            /**
             * @brief Constructs a window with the specified descriptor.
             *
             * @param desc Configuration parameters for the window.
             */
            explicit Window(WindowDescriptor desc = {}) noexcept;

            /**
             * @brief Creates the actual window and initializes GLFW context.
             *
             * @return int32_t 0 on success, non-zero on failure.
             */
            int32_t Create();

            /**
             * @brief Updates the window state and processes pending events.
             *
             * @details This method should be called once per frame to handle
             * window events and update the window state.
             */
            void Update();

            /**
             * @brief Destroys the window and cleans up resources.
             */
            void Destroy();

            /**
             * @brief Checks if the window should be closed.
             *
             * @return true if the window should close, false otherwise.
             */
            bool ShouldClose();

            /**
             * @brief Swaps the front and back buffers for double buffering.
             *
             * @details This method should be called after rendering to display
             * the rendered frame.
             */
            void SwapBuffers();

            /**
             * @brief Gets the internal GLFW window handle.
             *
             * @details Provides direct access to the GLFW window for advanced
             * operations that require GLFW-specific functionality.
             *
             * @return GLFWwindow* Pointer to the internal GLFW window.
             */
            GLFWwindow *GetHandle() const noexcept { return _internal; }

            /**
             * @brief Gets the window descriptor.
             *
             * @return const WindowDescriptor& Reference to the window configuration.
             */
            const WindowDescriptor &GetDescriptor() const noexcept;

        private:
            /**
             * @brief Subscribes to window events.
             *
             * @param e The window event to subscribe to.
             */
            void SubscribeEvent(const WindowEvent &e);

            /**
             * @brief Handles incoming window events.
             *
             * @param e The window event to process.
             */
            void HandleEvent(const WindowEvent &e);

            /**
             * @brief Handles window resize events.
             *
             * @param width New window width.
             * @param height New window height.
             */
            void HandleResize(int32_t width, int32_t height);

            /** @brief Window configuration parameters. */
            WindowDescriptor _desc;
            /** @brief Internal GLFW window handle. */
            GLFWwindow *_internal = nullptr;
            /** @brief Queue for managing window events. */
            WindowEventQueue _windowEventQueue;

            friend class Engine;
            friend class InternalEvents;
    };
}

#endif // WINDOW_HPP