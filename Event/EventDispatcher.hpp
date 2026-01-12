/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** EventDispatcher.hpp
*/

#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP
#include <Graphitech.hpp>
#include <set>
#include <vector>

#include "Event/Event.hpp"

namespace Graphitech {
    class EventHandler;

    /**
     * @brief Central event dispatcher managing event distribution.
     *
     * @details The EventDispatcher is responsible for:
     * - Registering and unregistering event handlers
     * - Dispatching events synchronously or asynchronously
     * - Managing a thread-safe event queue
     * - Coordinating event processing across multiple handlers
     *
     * Event handlers are processed in priority order, and events can be
     * dispatched immediately or queued for later processing.
     *
     * @ingroup Event
     */
    class EventDispatcher final {
        public:
            /**
             * @brief Destructor that cleans up all registered handlers.
             */
            ~EventDispatcher();

            /**
             * @brief Adds an event handler to the dispatcher.
             *
             * @details The handler will start receiving events. Handlers
             * are stored and will be notified of events in priority order.
             *
             * @param eventHandler The handler to add.
             */
            void AddEventHandler(EventHandler &eventHandler);

            /**
             * @brief Removes an event handler from the dispatcher.
             *
             * @details The handler will stop receiving events. The removal
             * is deferred until the next dispatch cycle to avoid issues
             * during event processing.
             *
             * @param eventHandler The handler to remove.
             */
            void RemoveEventHandler(EventHandler &eventHandler);

            /**
             * @brief Dispatches an event synchronously to all handlers.
             *
             * @details Immediately processes the event through all registered
             * handlers in priority order. Returns whether any handler
             * consumed the event.
             *
             * @param event The event to dispatch (ownership is transferred).
             * @return true if the event was handled by at least one handler.
             */
            bool DispatchEvent(std::unique_ptr<Event> event);

            /**
             * @brief Posts an event to the queue for asynchronous processing.
             *
             * @details Adds the event to the internal queue for later processing.
             * Returns a future that will be fulfilled when the event is processed.
             *
             * @param event The event to queue (ownership is transferred).
             * @return Future that resolves to whether the event was handled.
             */
            std::future<bool> PostEvent(std::unique_ptr<Event> event);

            /**
             * @brief Processes all queued events.
             *
             * @details Dispatches all events in the queue to registered handlers.
             * This method should be called regularly from the main thread.
             */
            void DispatchEvents();

        private:
            /** @brief Currently registered event handlers. */
            std::vector<EventHandler *> _eventHandlers;

            /** @brief Handlers pending addition (processed during next dispatch). */
            std::set<EventHandler *> _eventHandlerAdd;

            /** @brief Handlers pending removal (processed during next dispatch). */
            std::set<EventHandler *> _eventHandlerDel;

            /** @brief Thread-safe queue for asynchronous events. */
            EventQueue _eventQueue;
    };
}

#endif //EVENTDISPATCHER_HPP
