/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** TickEvent.hpp
*/

#ifndef TICKEVENT_HPP
#define TICKEVENT_HPP
#include <Event/Event.hpp>

namespace Graphitech {

    /**
     * @brief Event class for frame tick updates.
     *
     * @details TickEvent represents a frame update event that contains
     * the delta time since the last frame. This event is typically
     * dispatched once per frame to allow time-based updates in the
     * game loop and event handlers.
     *
     * @ingroup Time
     */
    class TickEvent final : public Event {
        public:
            /**
             * @brief Default constructor for tick events.
             *
             * @details Creates a tick event with EventId::Tick.
             */
            TickEvent() noexcept;

            /**
             * @brief Constructs a tick event with a specific event ID.
             *
             * @param id The event identifier (should be EventId::Tick).
             */
            explicit TickEvent(const EventId id) noexcept;

            /**
             * @brief Arguments structure containing tick event data.
             */
            struct Args final {
                /** @brief Time elapsed since the last frame in seconds. */
                float deltaTime = 0.f;
            } args;
    };
}

#endif //TICKEVENT_HPP
