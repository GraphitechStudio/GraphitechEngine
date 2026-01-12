/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** EventHandler.hpp
*/

#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#include "Event/IEventHandler.hpp"
#include "Time/TickEvent.hpp"
#include "Window/WindowEvent.hpp"

#define BIND_EVENT(DELEGATE) [this](auto&& arg) { return DELEGATE(std::forward<decltype(arg)>(arg)); }

namespace Graphitech {
    class EventDispatcher;
    class TickEvent;
    class KeyboardEvent;
    class MouseEvent;
    class WindowEvent;

    /**
     * @brief Base event handler class with built-in event type support.
     *
     * @details EventHandler provides a convenient base class for handling
     * common event types. It implements the IEventHandler interface and
     * provides function pointers for different event categories:
     * - Tick events (frame updates)
     * - Keyboard events (key press/release)
     * - Mouse events (buttons, motion, scroll)
     * - Window events (size changes)
     *
     * Handlers have a priority that determines processing order.
     *
     * @ingroup Event
     */
    class EventHandler : public IEventHandler {
        public:
            /**
             * @brief Destructor that automatically stops listening.
             */
            ~EventHandler() override;

            /**
             * @brief Constructs an event handler with specified priority.
             *
             * @details Higher priority handlers are processed first.
             * Default priority is 0.
             *
             * @param priority The handler priority (higher = processed first).
             */
            explicit EventHandler(const int32_t priority = 0) noexcept;

            /**
             * @brief Starts listening for events.
             *
             * @details Registers this handler with the global event dispatcher.
             * Must be called before events will be received.
             */
            virtual void StartListening() final;

            /**
             * @brief Stops listening for events.
             *
             * @details Unregisters this handler from the global event dispatcher.
             * No events will be received after this call.
             */
            virtual void StopListening() final;

        protected:
            /**
             * @brief Called when listening starts.
             *
             * @details Override to perform setup when event listening begins.
             */
            void OnStartListening() override;

            /**
             * @brief Called when listening stops.
             *
             * @details Override to perform cleanup when event listening ends.
             */
            void OnStopListening() override;

            /**
             * @brief Dispatches events to appropriate handlers.
             *
             * @details Routes events to the correct handler function based on
             * event type. Override for custom event routing logic.
             *
             * @param event The event to process.
             * @return true if the event was handled, false otherwise.
             */
            bool OnEventDispatch(Event *event) override;

            /** @brief Handler for tick events. Set to a lambda to handle frame updates. */
            std::function<bool(TickEvent)> TickHander = nullptr;

            /** @brief Handler for keyboard events. Set to a lambda to handle key input. */
            std::function<bool(KeyboardEvent)> KeyboardHandler = nullptr;

            /** @brief Handler for mouse events. Set to a lambda to handle mouse input. */
            std::function<bool(MouseEvent)> MouseHandler = nullptr;

            /** @brief Handler for window events. Set to a lambda to handle window changes. */
            std::function<bool(WindowEvent)> WindowHandler = nullptr;

        private:
            /** @brief Handler priority for processing order. */
            int32_t _priority;

            /** @brief Pointer to the event dispatcher (managed internally). */
            EventDispatcher *_eventDispatcher = nullptr;

            friend class EventDispatcher;
    };
}

#endif //EVENTHANDLER_HPP
