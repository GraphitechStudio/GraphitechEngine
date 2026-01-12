/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** MouseEvent.hpp
*/

#ifndef MOUSEEVENT_HPP
#define MOUSEEVENT_HPP

#include <Input/InputEvent.hpp>
#include <Input/Mouse/MouseButton.hpp>

namespace Graphitech {

    /**
     * @brief Event class for mouse input events.
     *
     * @details MouseEvent represents mouse-related input events such as
     * button presses/releases, cursor movement, and scroll wheel events.
     * It contains comprehensive information about the mouse state changes.
     *
     * @ingroup Input
     */
    class MouseEvent final : public InputEvent {
        public:
            /**
             * @brief Constructs a mouse event with the specified event ID.
             *
             * @param id The event identifier (MouseButtonPress, MouseButtonRelease, etc.).
             */
            explicit MouseEvent(const EventId id) noexcept;

            /**
             * @brief Arguments structure containing mouse event data.
             */
            struct Args final {
                /** @brief The mouse button involved in the event (for button events). */
                MouseButton button = MouseButton::Unknown;

                /** @brief X coordinate difference (for motion events). */
                float xDiff = 0.f;

                /** @brief Y coordinate difference (for motion events). */
                float yDiff = 0.f;

                /** @brief Absolute X coordinate of the mouse cursor. */
                float x = 0.f;

                /** @brief Absolute Y coordinate of the mouse cursor. */
                float y = 0.f;

                /** @brief Horizontal scroll amount (for scroll events). */
                float xScroll = 0.f;

                /** @brief Vertical scroll amount (for scroll events). */
                float yScroll = 0.f;

                /** @brief Whether this is relative motion (true) or absolute positioning (false). */
                bool relativeMotion = false;
            } args;
    };
}

#endif //MOUSEEVENT_HPP
