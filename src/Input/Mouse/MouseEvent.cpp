/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** MouseEvent.cpp
*/
#include <Input/Mouse/MouseEvent.hpp>
#include <Debugger/Debug.hpp>

MOD("Core.Input.Mouse.Event");

Graphitech::MouseEvent::MouseEvent(const EventId id) noexcept : InputEvent(id)
{
    switch (id) {
        case Graphitech::EventId::MouseButtonPress:
        case Graphitech::EventId::MouseButtonRelease:
        case Graphitech::EventId::MouseScroll:
        case Graphitech::EventId::MouseMotion:
            break;
        default:
            this->_id = EventId::Undefined;
            DEBUG_WARN("EventId is not from MouseEvent family. It will be set to Undefined!");
            break;
    }
}
