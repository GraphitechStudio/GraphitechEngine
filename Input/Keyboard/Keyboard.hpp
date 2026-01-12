/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Keyboard.hpp
*/

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
#include <Input/Keyboard/KeyCode.hpp>
#include <Graphitech.hpp>

namespace Graphitech {

    /**
     * @brief Manages keyboard input state and provides query interface.
     *
     * @details The Keyboard class tracks the state of all keyboard keys
     * and provides methods to query key states, convert keys to strings,
     * and handle keyboard events. It maintains an internal state array
     * for all supported keys.
     *
     * @ingroup Input
     */
    class Keyboard final {
        public:
            /**
             * @brief Checks if a specific key is currently pressed down.
             *
             * @param key The key code to check.
             * @return true if the key is pressed, false otherwise.
             */
            bool IsKeyDown(KeyCode key) const noexcept;

            /**
             * @brief Gets a string representation of all currently pressed keys.
             *
             * @details Returns a string containing the names of all keys
             * that are currently in the down state.
             *
             * @return String containing pressed key names.
             */
            std::string CharsDown();

            /**
             * @brief Converts a key code to its string representation.
             *
             * @param key The key code to convert.
             * @return String representation of the key (e.g., "A", "Space", "F1").
             */
            std::string KeyToString(KeyCode key);

        private:
            /**
             * @brief Handles a key press event.
             *
             * @param key The key that was pressed.
             * @param modifiers The modifier keys that were active.
             * @return true if the event was handled.
             */
            bool HandleKeyPress(KeyCode key, KeyModifiers modifiers);

            /**
             * @brief Handles a key release event.
             *
             * @param key The key that was released.
             * @param modifiers The modifier keys that were active.
             * @return true if the event was handled.
             */
            bool HandleKeyRelease(KeyCode key, KeyModifiers modifiers);

            /** @brief Array tracking the pressed state of all keys. */
            bool _keyStates[static_cast<uint32_t>(KeyCode::Count)]{false};

            /** @brief Currently active modifier keys. */
            KeyModifiers _modifiers = KeyModifiers::None;

            friend class Input;
            friend class InternalEvents;
    };
}

#endif //KEYBOARD_HPP
