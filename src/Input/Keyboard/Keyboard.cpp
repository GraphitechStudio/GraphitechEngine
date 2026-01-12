/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Keyboard.cpp
*/
#include <Input/Keyboard/Keyboard.hpp>
#include <Input/Keyboard/KeyboardEvent.hpp>
#include <Graphitech/Engine/Engine.hpp>

#include "Event/EventDispatcher.hpp"

bool Graphitech::Keyboard::IsKeyDown(KeyCode key) const noexcept
{
    return this->_keyStates[static_cast<uint32_t>(key)];
}

std::string Graphitech::Keyboard::CharsDown()
{
    std::string res;
    static KeyCode printableKeys[] = {
        KeyCode::Space, KeyCode::Apostrophe, KeyCode::Comma, KeyCode::Minus, KeyCode::Period, KeyCode::Slash,
        KeyCode::Key0, KeyCode::Key1, KeyCode::Key2, KeyCode::Key3, KeyCode::Key4, KeyCode::Key5, KeyCode::Key6,
        KeyCode::Key7, KeyCode::Key8, KeyCode::Key9, KeyCode::Semicolon, KeyCode::Equal, KeyCode::A, KeyCode::B,
        KeyCode::C, KeyCode::D, KeyCode::E, KeyCode::F, KeyCode::G, KeyCode::H, KeyCode::I, KeyCode::J, KeyCode::K,
        KeyCode::L, KeyCode::M, KeyCode::N, KeyCode::O, KeyCode::P, KeyCode::Q, KeyCode::R, KeyCode::S, KeyCode::T,
        KeyCode::U, KeyCode::V, KeyCode::W, KeyCode::X, KeyCode::Y, KeyCode::Z, KeyCode::LeftBracket,
        KeyCode::Backslash, KeyCode::RightBracket, KeyCode::GraveAccent
    };

    for (const auto &key: printableKeys)
        if (IsKeyDown(key))
            res += KeyToString(key);
    return res;
}

std::string Graphitech::Keyboard::KeyToString(KeyCode key)
{
    bool capsLock = static_cast<bool>(this->_modifiers & KeyModifiers::CapsLock);
    bool shift = static_cast<bool>(this->_modifiers & KeyModifiers::Shift);

    switch (key) {
        case KeyCode::Space:
            return " ";
        case KeyCode::Apostrophe:
            return shift ? "\"" : "'";
        case KeyCode::Comma:
            return shift ? "<" : ",";
        case KeyCode::Minus:
            return shift ? "_" : "-";
        case KeyCode::Period:
            return shift ? ">" : ".";
        case KeyCode::Slash:
            return shift ? "?" : "/";
        case KeyCode::Key0:
            return shift ? "!" : "0";
        case KeyCode::Key1:
            return shift ? "@" : "1";
        case KeyCode::Key2:
            return shift ? "#" : "2";
        case KeyCode::Key3:
            return shift ? "$" : "3";
        case KeyCode::Key4:
            return shift ? "%" : "4";
        case KeyCode::Key5:
            return shift ? "^" : "5";
        case KeyCode::Key6:
            return shift ? "&" : "6";
        case KeyCode::Key7:
            return shift ? "*" : "7";
        case KeyCode::Key8:
            return shift ? "(" : "8";
        case KeyCode::Key9:
            return shift ? ")" : "9";
        case KeyCode::Semicolon:
            return shift ? ":" : ";";
        case KeyCode::Equal:
            return shift ? "+" : "=";
        case KeyCode::A:
            return capsLock ? (shift ? "a" : "A") : (shift ? "A" : "a");
        case KeyCode::B:
            return capsLock ? (shift ? "b" : "B") : (shift ? "B" : "b");
        case KeyCode::C:
            return capsLock ? (shift ? "c" : "C") : (shift ? "C" : "c");
        case KeyCode::D:
            return capsLock ? (shift ? "d" : "D") : (shift ? "D" : "d");
        case KeyCode::E:
            return capsLock ? (shift ? "e" : "E") : (shift ? "E" : "e");
        case KeyCode::F:
            return capsLock ? (shift ? "f" : "F") : (shift ? "F" : "f");
        case KeyCode::G:
            return capsLock ? (shift ? "g" : "G") : (shift ? "G" : "g");
        case KeyCode::H:
            return capsLock ? (shift ? "h" : "H") : (shift ? "H" : "h");
        case KeyCode::I:
            return capsLock ? (shift ? "i" : "I") : (shift ? "I" : "i");
        case KeyCode::J:
            return capsLock ? (shift ? "j" : "J") : (shift ? "J" : "j");
        case KeyCode::K:
            return capsLock ? (shift ? "k" : "K") : (shift ? "K" : "k");
        case KeyCode::L:
            return capsLock ? (shift ? "l" : "L") : (shift ? "L" : "l");
        case KeyCode::M:
            return capsLock ? (shift ? "m" : "M") : (shift ? "M" : "m");
        case KeyCode::N:
            return capsLock ? (shift ? "n" : "N") : (shift ? "N" : "n");
        case KeyCode::O:
            return capsLock ? (shift ? "o" : "O") : (shift ? "O" : "o");
        case KeyCode::P:
            return capsLock ? (shift ? "p" : "P") : (shift ? "P" : "p");
        case KeyCode::Q:
            return capsLock ? (shift ? "q" : "Q") : (shift ? "Q" : "q");
        case KeyCode::R:
            return capsLock ? (shift ? "r" : "R") : (shift ? "R" : "r");
        case KeyCode::S:
            return capsLock ? (shift ? "s" : "S") : (shift ? "S" : "s");
        case KeyCode::T:
            return capsLock ? (shift ? "t" : "T") : (shift ? "T" : "t");
        case KeyCode::U:
            return capsLock ? (shift ? "u" : "U") : (shift ? "U" : "u");
        case KeyCode::V:
            return capsLock ? (shift ? "v" : "V") : (shift ? "V" : "v");
        case KeyCode::W:
            return capsLock ? (shift ? "w" : "W") : (shift ? "W" : "w");
        case KeyCode::X:
            return capsLock ? (shift ? "x" : "X") : (shift ? "X" : "x");
        case KeyCode::Y:
            return capsLock ? (shift ? "y" : "Y") : (shift ? "Y" : "y");
        case KeyCode::Z:
            return capsLock ? (shift ? "z" : "Z") : (shift ? "Z" : "z");
        case KeyCode::LeftBracket:
            return shift ? "{" : "[";
        case KeyCode::Backslash:
            return shift ? "|" : "\\";
        case KeyCode::RightBracket:
            return shift ? "}" : "]";
        case KeyCode::GraveAccent:
            return shift ? "~" : "`";
        default:
            return "";
    }
}

bool Graphitech::Keyboard::HandleKeyPress(KeyCode key, KeyModifiers modifiers)
{
    if (this->_keyStates[static_cast<uint32_t>(key)])
        return false;

    auto event = std::make_unique<KeyboardEvent>(EventId::KeyPress);

    event->args.key = key;
    event->args.modifiers = modifiers;

    this->_keyStates[static_cast<uint32_t>(key)] = true;
    this->_modifiers = modifiers;

    return Engine::_eventDispatcher->DispatchEvent(std::move(event));
}
bool Graphitech::Keyboard::HandleKeyRelease(KeyCode key, KeyModifiers modifiers)
{
    auto event = std::make_unique<KeyboardEvent>(EventId::KeyRelease);

    event->args.key = key;
    event->args.modifiers = modifiers;

    this->_keyStates[static_cast<uint32_t>(key)] = false;
    this->_modifiers = modifiers;

    return Engine::_eventDispatcher->DispatchEvent(std::move(event));
}
