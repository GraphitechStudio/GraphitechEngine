/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Input.hpp
*/

#ifndef INPUT_HPP
#define INPUT_HPP
#include <Input/InputEvent.hpp>
#include <Input/Keyboard/KeyCode.hpp>

namespace Graphitech {
    class Keyboard;
    class Mouse;

    /**
     * @brief Central input system manager.
     *
     * @details The Input class coordinates all input devices and manages
     * the input event processing. It provides global access to keyboard
     * and mouse instances and handles the input event queue processing.
     * This class is responsible for initializing input devices and
     * dispatching input events to the appropriate handlers.
     *
     * @ingroup Input
     */
    enum class KeyboardLayout { QWERTY, AZERTY };

    class Input final {
        public:
            /** @brief Global keyboard input device instance. */
            inline static Keyboard *_keyboard = nullptr;

            /** @brief Global mouse input device instance. */
            inline static Mouse *_mouse = nullptr;

            /** @brief Set keyboard layout preference (QWERTY/AZERTY). */
            static void SetKeyboardLayout(KeyboardLayout l) { _layout = l; }
            static KeyboardLayout GetKeyboardLayout() { return _layout; }

            /** @brief Remap a KeyCode according to current keyboard layout. */
            static KeyCode RemapKeyForLayout(KeyCode k);

        private:
            /**
             * @brief Registers and initializes all input devices.
             *
             * @details Called during engine initialization to set up
             * keyboard and mouse devices.
             */
            static void RegisterDevices();

            /**
             * @brief Releases all input device resources.
             *
             * @details Called during engine shutdown to clean up
             * input device resources.
             */
            static void ReleaseDevices();

            /**
             * @brief Updates the input system for the current frame.
             *
             * @details Processes pending input events and updates
             * device states. Called once per frame by the engine.
             */
            static void Update();

            /**
             * @brief Handles an input event.
             *
             * @details Dispatches the input event to the appropriate
             * device handler for processing.
             *
             * @param e The input event to handle.
             * @return true if the event was processed.
             */
            static bool HandleEvent(std::unique_ptr<InputEvent> e);

            /** @brief Thread-safe queue for input events. */
            inline static InputEventQueue _inputEventQueue;

            /** Current keyboard layout. */
            static KeyboardLayout _layout;

            friend class Engine;
    };
}

#endif //INPUT_HPP
