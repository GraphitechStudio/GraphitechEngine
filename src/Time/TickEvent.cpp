/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** TickEvent.cpp
*/
#include <Time/TickEvent.hpp>

Graphitech::TickEvent::TickEvent() noexcept : Event(EventId::Undefined) {}
Graphitech::TickEvent::TickEvent(const EventId id) noexcept : Event(id) {}
