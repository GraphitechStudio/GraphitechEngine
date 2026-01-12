/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** KeyboardEvent.hpp
*/

#ifndef KEYBOARDEVENT_HPP
#define KEYBOARDEVENT_HPP

#include <Input/InputEvent.hpp>
#include <Input/Keyboard/KeyCode.hpp>

namespace Graphitech {

    /**
     * @brief Event class for keyboard input events.
     *
     * @details KeyboardEvent represents keyboard-related input events such as
     * key presses and releases. It contains information about which key was
     * involved and any modifier keys that were active at the time of the event.
     *
     * @ingroup Input
     */
    class KeyboardEvent final : public InputEvent {
        public:
            /**
             * @brief Constructs a keyboard event with the specified event ID.
             *
             * @param id The event identifier (KeyPress or KeyRelease).
             */
            explicit KeyboardEvent(const EventId id) noexcept;

            /**
             * @brief Arguments structure containing keyboard event data.
             */
            struct Args final {
                /** @brief The key that triggered the event. */
                KeyCode key = KeyCode::Unknown;

                /** @brief Modifier keys that were active during the event. */
                KeyModifiers modifiers = KeyModifiers::None;
            } args;
    };
}

#endif //KEYBOARDEVENT_HPP
