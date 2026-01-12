/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** EventDispatcher.cpp
*/
#include <Event/EventDispatcher.hpp>
#include <Event/EventHandler.hpp>
#include <algorithm>

Graphitech::EventDispatcher::~EventDispatcher()
{
    for (const auto &eventHandler : this->_eventHandlerAdd)
        eventHandler->_eventDispatcher = nullptr;
    for (const auto &eventHandler : this->_eventHandlers) {
        const auto &it = std::find(this->_eventHandlerDel.begin(), this->_eventHandlerDel.end(), eventHandler);

        if (it == this->_eventHandlerDel.end())
            eventHandler->_eventDispatcher = nullptr;
    }
}

void Graphitech::EventDispatcher::AddEventHandler(EventHandler &eventHandler)
{
    if (eventHandler._eventDispatcher != nullptr)
        eventHandler._eventDispatcher->RemoveEventHandler(eventHandler);
    eventHandler._eventDispatcher = this;
    this->_eventHandlerAdd.insert(&eventHandler);

    const auto &it = this->_eventHandlerDel.find(&eventHandler);
    if (it != this->_eventHandlerDel.end())
        this->_eventHandlerDel.erase(it);
}
void Graphitech::EventDispatcher::RemoveEventHandler(EventHandler &eventHandler)
{
    if (eventHandler._eventDispatcher == this)
        eventHandler._eventDispatcher = nullptr;

    this->_eventHandlerDel.insert(&eventHandler);

    const auto &it = this->_eventHandlerAdd.find(&eventHandler);
    if (it != this->_eventHandlerAdd.end())
        this->_eventHandlerAdd.erase(it);
}
bool Graphitech::EventDispatcher::DispatchEvent(std::unique_ptr<Event> event)
{
    if (!event)
        return false;
    bool handled = false;

    for (const auto &eventHandler : this->_eventHandlers) {
        const auto &it = std::find(this->_eventHandlerDel.begin(), this->_eventHandlerDel.end(), eventHandler);

        if (it == this->_eventHandlerDel.end())
            handled = eventHandler->OnEventDispatch(event.get());
        if (handled)
            break;
    }
    return handled;
}
std::future<bool> Graphitech::EventDispatcher::PostEvent(std::unique_ptr<Event> event)
{
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();
    std::lock_guard lock { this->_eventQueue.mutex };

    this->_eventQueue.queue.push({ std::move(promise), std::move(event) });
    return future;
}
void Graphitech::EventDispatcher::DispatchEvents()
{
    for (const auto &eventHandler : this->_eventHandlerDel) {
        const auto &it = std::find(this->_eventHandlers.begin(), this->_eventHandlers.end(), eventHandler);

        if (it != this->_eventHandlers.end())
            this->_eventHandlers.erase(it);
    }

    this->_eventHandlerDel.clear();

    for (const auto &eventHandler : this->_eventHandlerAdd) {
        const auto &it = std::find(this->_eventHandlers.begin(), this->_eventHandlers.end(), eventHandler);

        if (it == this->_eventHandlers.end()) {
            auto upperBound = std::upper_bound(this->_eventHandlers.begin(), this->_eventHandlers.end(), eventHandler, [](const auto &first, const auto &second) noexcept {
                return first->_priority > second->_priority;
            });

            this->_eventHandlers.insert(upperBound, eventHandler);
        }
    }

    this->_eventHandlerAdd.clear();

    EventQueue::EventType event;

    while (true) {
        std::unique_lock<std::mutex> lock{ this->_eventQueue.mutex };

        if (this->_eventQueue.queue.empty())
            break;

        event = std::move(this->_eventQueue.queue.front());
        this->_eventQueue.queue.pop();
        lock.unlock();

        event.first.set_value(DispatchEvent(std::move(event.second)));
    }
}


