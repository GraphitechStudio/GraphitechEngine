/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** KeyboardEvent.cpp
*/
#include <Input/Keyboard/KeyboardEvent.hpp>
#include <Debugger/Debug.hpp>

MOD("Core.Input.Keyboard.Event");

Graphitech::KeyboardEvent::KeyboardEvent(const EventId id) noexcept : InputEvent(id)
{
    switch (id) {
        case Graphitech::EventId::KeyPress:
        case Graphitech::EventId::KeyRelease:
            break;
        default:
            this->_id = EventId::Undefined;
            DEBUG_WARN("EventId is not from KeyboardEvent. It will be set to Undefined");
            break;
    }
}
