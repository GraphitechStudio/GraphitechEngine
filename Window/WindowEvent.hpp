/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** WindowEvent.hpp
*/

#ifndef WINDOWEVENT_HPP
#define WINDOWEVENT_HPP
#include <Event/Event.hpp>

namespace Graphitech {
    class Window;

    /**
     * @brief Event class for window-related notifications.
     *
     * @details WindowEvent represents events related to window state changes,
     * such as resizing, moving, or other window operations. It extends the
     * base Event class and includes window-specific arguments.
     *
     * @ingroup Window
     */
    class WindowEvent final : public Event {
        public:
            /**
             * @brief Default constructor for window events.
             *
             * @details Creates a window event with EventId::Window.
             */
            WindowEvent() noexcept;

            /**
             * @brief Constructs a window event with a specific event ID.
             *
             * @param id The event identifier (should be EventId::Window).
             */
            explicit WindowEvent(const EventId id) noexcept;

            /**
             * @brief Arguments structure containing window event data.
             */
            struct Args final {
                /** @brief Window width in pixels (used for resize events). */
                uint32_t width = 0u;
                /** @brief Window height in pixels (used for resize events). */
                uint32_t height = 0u;
            } args;

            /** @brief Pointer to the window that generated the event. */
            Window *window = nullptr;
    };

    /**
     * @brief Thread-safe queue for managing window events.
     *
     * @details WindowEventQueue provides a synchronized queue for storing
     * and retrieving window events in a multi-threaded environment.
     */
    struct WindowEventQueue final {
        /** @brief Queue containing window events. */
        std::queue<WindowEvent> queue;
        /** @brief Mutex for thread-safe access to the queue. */
        std::mutex mutex;
    };
}

#endif //WINDOWEVENT_HPP
