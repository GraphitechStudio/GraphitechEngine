/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** IEventHandler.hpp
*/

#ifndef IEVENTHANDLER_HPP
#define IEVENTHANDLER_HPP
#include "Event/Event.hpp"

namespace Graphitech {

    /**
     * @brief Interface for objects that can handle events.
     *
     * @details IEventHandler defines the contract for event handling objects.
     * It provides methods for managing the listening state (start/stop)
     * and for dispatching events to the handler.
     *
     * Event handlers typically register with an EventDispatcher to receive
     * events of interest.
     *
     * @ingroup Event
     */
    class IEventHandler {
        public:
            /**
             * @brief Virtual destructor for proper cleanup.
             */
            virtual ~IEventHandler() = default;

        protected:
            /**
             * @brief Called when the handler starts listening for events.
             *
             * @details Override this method to perform setup when the handler
             * begins receiving events (e.g., register with systems).
             */
            virtual void OnStartListening() = 0;

            /**
             * @brief Called when the handler stops listening for events.
             *
             * @details Override this method to perform cleanup when the handler
             * stops receiving events (e.g., unregister from systems).
             */
            virtual void OnStopListening() = 0;

            /**
             * @brief Called to dispatch an event to this handler.
             *
             * @details This method is called by the event dispatcher when
             * an event needs to be processed by this handler.
             *
             * @param event The event to process.
             * @return true if the event was handled/consumed, false otherwise.
             */
            virtual bool OnEventDispatch(Event *event) = 0;
    };
}

#endif //IEVENTHANDLER_HPP
