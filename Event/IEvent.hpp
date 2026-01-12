/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** IEvent.hpp
*/

#ifndef IEVENT_HPP
#define IEVENT_HPP

namespace Graphitech {

    /**
     * @brief Interface for event objects with lifecycle management.
     *
     * @details IEvent defines the basic interface that all event types
     * must implement. It provides virtual methods for event creation
     * and release, allowing events to perform initialization and cleanup.
     *
     * Events are typically created on the heap and managed via unique_ptr.
     *
     * @ingroup Event
     */
    class IEvent {
        public:
            /**
             * @brief Virtual destructor for proper cleanup.
             */
            virtual ~IEvent() = default;

        protected:
            /**
             * @brief Called when the event is created.
             *
             * @details Override this method to perform any initialization
             * logic when the event is constructed.
             */
            virtual void OnCreate() = 0;

            /**
             * @brief Called when the event is released.
             *
             * @details Override this method to perform any cleanup
             * logic when the event is destroyed.
             */
            virtual void OnRelease() = 0;
    };
}

#endif //IEVENT_HPP