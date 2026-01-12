/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** WindowEvent.cpp
*/
#include <Window/WindowEvent.hpp>
#include <Debugger/Debug.hpp>
#include <Graphitech.hpp>

MOD("Core.Window.Event");
Graphitech::WindowEvent::WindowEvent() noexcept : Event(EventId::Undefined) {}
Graphitech::WindowEvent::WindowEvent(const EventId id) noexcept : Event(id)
{
    switch (id) {
        case Graphitech::EventId::Window_Size_Change:
            break;
        default:
            this->_id = EventId::Undefined;
            DEBUG_WARN("EventId is not from WindowEvent family. It will be set to Undefined!");
            break;
    }
}
