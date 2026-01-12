/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** InputEvent.cpp
*/
#include <Input/InputEvent.hpp>
Graphitech::InputEvent::InputEvent() noexcept : Event(EventId::Undefined) {}
Graphitech::InputEvent::InputEvent(const EventId id) noexcept : Event(id) {}
