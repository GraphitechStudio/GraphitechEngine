/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Mouse.hpp
*/

#ifndef MOUSE_HPP
#define MOUSE_HPP
#include <cstdint>
#include <Input/Mouse/MouseButton.hpp>

namespace Graphitech {

    /**
     * @brief Manages mouse input state and provides query interface.
     *
     * @details The Mouse class tracks mouse position and button states,
     * providing methods to query current mouse position and button presses.
     * It handles various mouse events including motion, button presses,
     * releases, and scroll wheel events.
     *
     * @ingroup Input
     */
    class Mouse final {
        public:
            /**
             * @brief Gets the current X coordinate of the mouse cursor.
             *
             * @return The X position of the mouse cursor.
             */
            float GetX() const noexcept;

            /**
             * @brief Gets the current Y coordinate of the mouse cursor.
             *
             * @return The Y position of the mouse cursor.
             */
            float GetY() const noexcept;

            /**
             * @brief Checks if a specific mouse button is currently pressed.
             *
             * @param button The mouse button to check.
             * @return true if the button is pressed, false otherwise.
             */
            bool IsButtonDown(MouseButton button ) const;

        private:
            /**
             * @brief Handles a mouse button press event.
             *
             * @param button The button that was pressed.
             * @param x The X coordinate where the press occurred.
             * @param y The Y coordinate where the press occurred.
             * @return true if the event was handled.
             */
            bool HandleButtonPress(MouseButton button, float x, float y);

            /**
             * @brief Handles a mouse button release event.
             *
             * @param button The button that was released.
             * @param x The X coordinate where the release occurred.
             * @param y The Y coordinate where the release occurred.
             * @return true if the event was handled.
             */
            bool HandleButtonRelease(MouseButton button, float x, float y);

            /**
             * @brief Handles mouse motion events.
             *
             * @param x The new X coordinate of the mouse.
             * @param y The new Y coordinate of the mouse.
             * @return true if the event was handled.
             */
            bool HandleMotion(float x, float y);

            /**
             * @brief Handles relative mouse motion events.
             *
             * @param x The relative X movement.
             * @param y The relative Y movement.
             * @return true if the event was handled.
             */
            bool HandleRelativeMotion(float x, float y);

            /**
             * @brief Handles mouse scroll wheel events.
             *
             * @param xScroll The horizontal scroll amount.
             * @param yScroll The vertical scroll amount.
             * @param x The X coordinate of the mouse during scroll.
             * @param y The Y coordinate of the mouse during scroll.
             * @return true if the event was handled.
             */
            bool HandleScroll(float xScroll, float yScroll, float x, float y);

            /** @brief Current X coordinate of the mouse cursor. */
            float _x = 0.f;

            /** @brief Current Y coordinate of the mouse cursor. */
            float _y = 0.f;

            /** @brief Array tracking the pressed state of all mouse buttons. */
            bool _buttonState[static_cast<uint32_t>(MouseButton::Count)]{false};

            friend class Input;
            friend class InternalEvents;
    };
}

#endif //MOUSE_HPP
