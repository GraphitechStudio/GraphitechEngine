/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Event.hpp
*/
#ifndef EVENT_HPP
#define EVENT_HPP
#include <queue>

#include "Graphitech.hpp"
#include "Event/IEvent.hpp"

namespace Graphitech {

    /**
     * @brief Enumeration of all supported event types.
     *
     * @details Defines unique identifiers for different categories of events:
     * - Process events (tick, timing)
     * - Keyboard events (press/release)
     * - Mouse events (buttons, motion, scroll)
     * - Window events (size changes)
     */
    enum class EventId : gId_t {
        Undefined,          /**< Undefined/invalid event */

        //Process Events
        Tick,               /**< Frame tick event */

        //Keyboard Events
        KeyPress,           /**< Keyboard key pressed */
        KeyRelease,         /**< Keyboard key released */

        // Mouse Events
        MouseButtonPress,   /**< Mouse button pressed */
        MouseButtonRelease, /**< Mouse button released */
        MouseMotion,        /**< Mouse movement */
        MouseScroll,        /**< Mouse scroll wheel */

        //Window Events
        Window_Size_Change  /**< Window size changed */
    };

    /**
     * @brief Base class for all events in the system.
     *
     * @details All concrete event types inherit from this class. It provides
     * the basic event lifecycle (creation/release) and stores the event ID.
     * Events are designed to be created on the heap and managed via unique_ptr.
     *
     * @ingroup Event
     */
    class Event : public IEvent {
        public:
            /**
             * @brief Gets the event identifier.
             *
             * @return The event type identifier.
             */
            EventId getId() const noexcept;

        protected:
            /**
             * @brief Constructs an event with the specified ID.
             *
             * @param id The event type identifier.
             */
            explicit Event(const EventId id) noexcept;

            /**
             * @brief Called when the event is created.
             *
             * @details Override this to perform initialization logic.
             */
            void OnCreate() override;

            /**
             * @brief Called when the event is released.
             *
             * @details Override this to perform cleanup logic.
             */
            void OnRelease() override;

            /** @brief The event type identifier. */
            EventId _id = EventId::Undefined;
    };

    /**
     * @brief Thread-safe event queue structure.
     *
     * @details Provides a queue for storing events with their associated
     * completion promises. Uses a mutex for thread-safe access.
     * Each queued event is paired with a promise that will be fulfilled
     * when the event is processed.
     *
     * @ingroup Event
     */
    struct EventQueue final {
        /**
         * @brief Type alias for queued event entries.
         *
         * Each entry contains a promise for completion notification
         * and the event itself.
         */
        using EventType = std::pair<std::promise<bool>, std::unique_ptr<Event>>;

        /** @brief The actual event queue. */
        std::queue<EventType> queue;

        /** @brief Mutex for thread-safe queue access. */
        std::mutex mutex;
    };
}

#endif //EVENT_HPP
