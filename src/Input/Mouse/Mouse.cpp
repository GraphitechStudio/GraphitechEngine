/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Mouse.cpp
*/
#include <Input/Mouse/Mouse.hpp>
#include <Input/Mouse/MouseEvent.hpp>
#include <Graphitech/Engine/Engine.hpp>
#include <Event/EventDispatcher.hpp>

float Graphitech::Mouse::GetX() const noexcept { return this->_x; }
float Graphitech::Mouse::GetY() const noexcept { return this->_y; }
bool Graphitech::Mouse::IsButtonDown(MouseButton button) const { return this->_buttonState[static_cast<uint32_t>(button)]; }
bool Graphitech::Mouse::HandleButtonPress(MouseButton button, float x, float y)
{
    auto event = std::make_unique<MouseEvent>(EventId::MouseButtonPress);
    event->args.button = button;
    event->args.x = x;
    event->args.y = y;

    this->_buttonState[static_cast<uint32_t>(button)] = true;

    return Engine::_eventDispatcher->DispatchEvent(std::move(event));
}
bool Graphitech::Mouse::HandleButtonRelease(MouseButton button, float x, float y)
{
    auto event = std::make_unique<MouseEvent>(EventId::MouseButtonRelease);
    event->args.button = button;
    event->args.x = x;
    event->args.y = y;

    this->_buttonState[static_cast<uint32_t>(button)] = false;

    return Engine::_eventDispatcher->DispatchEvent(std::move(event));
}
bool Graphitech::Mouse::HandleMotion(float x, float y)
{
    auto event = std::make_unique<MouseEvent>(EventId::MouseMotion);
    event->args.xDiff = x - this->_x;
    event->args.yDiff = y - this->_y;
    event->args.x = x;
    event->args.y = y;

    this->_x = x;
    this->_y = y;
    return Engine::_eventDispatcher->DispatchEvent(std::move(event));
}
bool Graphitech::Mouse::HandleRelativeMotion(float x, float y)
{
    float xNewPos = this->_x + x;
    float yNewPos = this->_y + y;

    xNewPos = std::clamp(xNewPos, 0.f, 1.0f);
    yNewPos = std::clamp(yNewPos, 0.f, 1.0f);

    return HandleMotion(xNewPos, yNewPos);
}
bool Graphitech::Mouse::HandleScroll(float xScroll, float yScroll, float x, float y)
{
    auto event = std::make_unique<MouseEvent>(EventId::MouseScroll);

    event->args.xScroll = xScroll;
    event->args.yScroll = yScroll;
    event->args.x = x;
    event->args.y = y;

    return Engine::_eventDispatcher->DispatchEvent(std::move(event));
}
