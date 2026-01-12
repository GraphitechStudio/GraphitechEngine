/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** EventHandler.cpp
*/
#include <Event/EventHandler.hpp>
#include <Event/EventDispatcher.hpp>
#include <Input/Keyboard/KeyboardEvent.hpp>
#include <Input/Mouse/MouseEvent.hpp>
#include <Window/WindowEvent.hpp>
#include <Graphitech/Engine/Engine.hpp>

Graphitech::EventHandler::~EventHandler()
{
    if (this->_eventDispatcher != nullptr)
        this->_eventDispatcher->RemoveEventHandler(*this);
}

Graphitech::EventHandler::EventHandler(const int32_t priority) noexcept : _priority(priority),
                                                                    _eventDispatcher(
                                                                        Graphitech::Engine::_eventDispatcher) {}

void Graphitech::EventHandler::StartListening()
{
    this->_eventDispatcher->AddEventHandler(*this);
    this->OnStartListening();
}

void Graphitech::EventHandler::StopListening()
{
    if (this->_eventDispatcher != nullptr) {
        this->_eventDispatcher->RemoveEventHandler(*this);
        this->_eventDispatcher = nullptr;
    }

    this->OnStopListening();
}

void Graphitech::EventHandler::OnStartListening() {}
void Graphitech::EventHandler::OnStopListening() {}

bool Graphitech::EventHandler::OnEventDispatch(Event *event)
{
    bool res = false;

    switch (event->getId()) {
        case EventId::Undefined:
            break;
        case EventId::KeyPress:
        case EventId::KeyRelease: {
            if (KeyboardHandler != nullptr) { res = KeyboardHandler(*dynamic_cast<KeyboardEvent *>(event)); }
            break;
        }
        case EventId::MouseButtonPress:
        case EventId::MouseButtonRelease:
        case EventId::MouseMotion:
        case EventId::MouseScroll: {
            if (MouseHandler != nullptr) { res = MouseHandler(*dynamic_cast<MouseEvent *>(event)); }
            break;
        }
        case EventId::Window_Size_Change: {
            if (this->WindowHandler != nullptr)
                res = WindowHandler(*dynamic_cast<WindowEvent *>(event));
            break;
        }
        case EventId::Tick: {
            if (TickHander != nullptr)
                res = TickHander(*dynamic_cast<TickEvent *>(event));
            break;
        }
        default: ;
    }
    return res;
}
