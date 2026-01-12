/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** Event.cpp
*/
#include <Event/Event.hpp>

Graphitech::EventId Graphitech::Event::getId() const noexcept { return this->_id; }
Graphitech::Event::Event(const EventId id) noexcept : _id(id) {}
void Graphitech::Event::OnCreate() {}
void Graphitech::Event::OnRelease() {}
