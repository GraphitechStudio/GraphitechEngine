/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** InputEvent.hpp
*/

#ifndef INPUTEVENT_HPP
#define INPUTEVENT_HPP

#include <Event/Event.hpp>

namespace Graphitech {

    /**
     * @brief Base class for all input-related events.
     *
     * @details InputEvent serves as the base class for all events generated
     * by input devices (keyboard, mouse, etc.). It inherits from the core
     * Event class and provides input-specific event handling.
     *
     * @ingroup Input
     */
    class InputEvent : public Event {
        public:
            /**
             * @brief Default constructor for input events.
             *
             * @details Creates an input event with undefined ID.
             */
            InputEvent() noexcept;

            /**
             * @brief Constructs an input event with a specific ID.
             *
             * @param id The event identifier.
             */
            explicit InputEvent(const EventId id) noexcept;
    };

    /**
     * @brief Thread-safe queue structure for input events.
     *
     * @details InputEventQueue provides a thread-safe mechanism for queuing
     * input events with their associated completion promises. Similar to
     * EventQueue but specialized for input events.
     *
     * @ingroup Input
     */
    struct InputEventQueue final {
        /**
         * @brief Type alias for queued input event entries.
         *
         * Each entry contains a promise for completion notification
         * and the input event itself.
         */
        using EventType = std::pair<std::promise<bool>, std::unique_ptr<InputEvent>>;

        /** @brief The actual input event queue. */
        std::queue<EventType> queue;

        /** @brief Mutex for thread-safe queue access. */
        std::mutex mutex;
    };
}

#endif //INPUTEVENT_HPP
