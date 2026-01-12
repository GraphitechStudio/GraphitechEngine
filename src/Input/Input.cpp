/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Input.cpp
*/
#include <Input/Input.hpp>
#include <Input/Keyboard/Keyboard.hpp>
#include <Input/Keyboard/KeyboardEvent.hpp>
#include <Input/Mouse/Mouse.hpp>
#include <Input/Mouse/MouseEvent.hpp>
#include <Debugger/Debug.hpp>

MOD("Core.Input");

Graphitech::KeyboardLayout Graphitech::Input::_layout = Graphitech::KeyboardLayout::QWERTY;

void Graphitech::Input::RegisterDevices()
{
    _keyboard = new Keyboard();
    _mouse = new Mouse();
}

Graphitech::KeyCode Graphitech::Input::RemapKeyForLayout(KeyCode k) {
    if (_layout == KeyboardLayout::QWERTY) return k;

    // AZERTY remapping for common letter keys: swap A<->Q, Z<->W, M<->, etc.
    switch (k) {
        case KeyCode::A: return KeyCode::Q;
        case KeyCode::Q: return KeyCode::A;
        case KeyCode::Z: return KeyCode::W;
        case KeyCode::W: return KeyCode::Z;
        case KeyCode::M: return KeyCode::Semicolon; // approximate
        default: return k;
    }
}
void Graphitech::Input::ReleaseDevices()
{
    SAFE_RELEASE(_mouse);
    SAFE_RELEASE(_keyboard);
}

void Graphitech::Input::Update()
{
    InputEventQueue::EventType event;

    while (true) {
        std::unique_lock<std::mutex> lock{_inputEventQueue.mutex};

        if (_inputEventQueue.queue.empty())
            break;
        event = std::move(_inputEventQueue.queue.front());
        _inputEventQueue.queue.pop();
        lock.unlock();

        event.first.set_value(HandleEvent(std::move(event.second)));
    }
}

bool Graphitech::Input::HandleEvent(std::unique_ptr<InputEvent> e)
{
    switch (e->getId()) {
        case EventId::KeyPress: {
            auto keyboardEvent = dynamic_cast<KeyboardEvent *>(e.get());
            return _keyboard->HandleKeyPress(keyboardEvent->args.key, keyboardEvent->args.modifiers);
        }
        case EventId::KeyRelease: {
            auto keyboardEvent = dynamic_cast<KeyboardEvent *>(e.get());
            return _keyboard->HandleKeyRelease(keyboardEvent->args.key, keyboardEvent->args.modifiers);
        }
        case EventId::MouseButtonPress: {
            auto mouseEvent = dynamic_cast<MouseEvent *>(e.get());
            return _mouse->HandleButtonPress(mouseEvent->args.button, mouseEvent->args.x, mouseEvent->args.y);
        }
        case EventId::MouseButtonRelease: {
            auto mouseEvent = dynamic_cast<MouseEvent *>(e.get());
            return _mouse->HandleButtonRelease(mouseEvent->args.button, mouseEvent->args.x, mouseEvent->args.y);
        }
        case EventId::MouseMotion: {
            auto mouseEvent = dynamic_cast<MouseEvent *>(e.get());
            return mouseEvent->args.relativeMotion
                       ? _mouse->HandleRelativeMotion(mouseEvent->args.x, mouseEvent->args.y)
                       : _mouse->HandleMotion(mouseEvent->args.x, mouseEvent->args.y);
        }
        case EventId::MouseScroll: {
            auto mouseEvent = dynamic_cast<MouseEvent *>(e.get());
            return _mouse->HandleScroll(mouseEvent->args.xScroll, mouseEvent->args.yScroll, mouseEvent->args.x,
                                        mouseEvent->args.y);
        }
        default: { DEBUG_WARN("Unhandled Event!"); }
    }
    return false;
}
