/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** InternalEvent.hpp
*/

#ifndef INTERNALEVENT_HPP
#define INTERNALEVENT_HPP
#include "Window/WindowEvent.hpp"

namespace Graphitech {

    /**
     * @brief Static class that bridges GLFW callbacks to engine events.
     *
     * @details InternalEvents provides static methods to register GLFW
     * callbacks and convert them to engine events. It handles:
     * - Framebuffer size changes (window resize)
     * - Keyboard input (key press/release)
     * - Mouse button events
     * - Mouse motion events
     * - Mouse scroll events
     *
     * This class is used internally by the Window class to connect
     * GLFW's C-style callbacks to the engine's C++ event system.
     *
     * @ingroup Event
     */
    class InternalEvents final {
        public:
            /**
             * @brief Registers all GLFW callbacks for a window.
             *
             * @details Sets up GLFW callback functions for the specified window.
             * This connects GLFW's event system to the engine's event dispatcher.
             *
             * @param internalWindow The GLFW window to register callbacks for.
             */
            static void Register(GLFWwindow *internalWindow);

            /**
             * @brief GLFW callback for framebuffer size changes.
             *
             * @details Called by GLFW when the window is resized. Converts
             * the event to a Window_Size_Change event in the engine.
             *
             * @param windowHandle The GLFW window that changed size.
             * @param width The new window width.
             * @param height The new window height.
             */
            static void FrameBufferSizeCallback(GLFWwindow *windowHandle, int width, int height);

            /**
             * @brief GLFW callback for keyboard events.
             *
             * @details Called by GLFW for key press/release events. Converts
             * to KeyPress or KeyRelease events in the engine.
             *
             * @param internalWindow The GLFW window.
             * @param key The GLFW key code.
             * @param scanCode The platform-specific scan code.
             * @param action GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT.
             * @param modifiers Bit field of modifier keys.
             */
            static void KeyCallback(GLFWwindow *internalWindow, int32_t key, int32_t scanCode, int32_t action, int32_t modifiers);

            /**
             * @brief GLFW callback for mouse button events.
             *
             * @details Called by GLFW for mouse button press/release. Converts
             * to MouseButtonPress or MouseButtonRelease events.
             *
             * @param internalWindow The GLFW window.
             * @param button The mouse button that changed.
             * @param action GLFW_PRESS or GLFW_RELEASE.
             * @param modifiers Bit field of modifier keys.
             */
            static void MouseButtonCallback(GLFWwindow *internalWindow, int32_t button, int32_t action, int32_t modifiers);

            /**
             * @brief GLFW callback for mouse motion events.
             *
             * @details Called by GLFW when the mouse moves. Converts to
             * MouseMotion events in the engine.
             *
             * @param internalWindow The GLFW window.
             * @param xPosition The new mouse X position.
             * @param yPosition The new mouse Y position.
             */
            static void MouseMotionCallback(GLFWwindow *internalWindow, double xPosition, double yPosition);

            /**
             * @brief GLFW callback for mouse scroll events.
             *
             * @details Called by GLFW for mouse wheel scrolling. Converts to
             * MouseScroll events in the engine.
             *
             * @param internalWindow The GLFW window.
             * @param xScroll Horizontal scroll offset.
             * @param yScroll Vertical scroll offset.
             */
            static void MouseScrollCallback(GLFWwindow *internalWindow, double xScroll, double yScroll);
    };
}

#endif //INTERNALEVENT_HPP
