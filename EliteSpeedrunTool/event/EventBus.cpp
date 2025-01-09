#include "EventBus.h"

Q_GLOBAL_STATIC(EventBus, eventBusInstance)

EventBus::EventBus(QObject* parent)
    : QObject { parent }
{
}

EventBus::~EventBus()
{
    observers.clear();
}

EventBus* EventBus::instance()
{
    return eventBusInstance;
}

void EventBus::send(Event* event)
{
    for (auto o = observers.cbegin(), end = observers.cend(); o != end; o++) {
        (*o)->onEvent(event);
    }
}

void EventBus::addObserver(EventObserver* observer)
{
    observers.insert(observer);
}

void EventBus::removeObserver(EventObserver* observer)
{
    observers.remove(observer);
}

Event::Event(QObject* parent)
    : QObject { parent }
{
}

EventObserver::EventObserver(QObject* parent)
    : QObject { parent }
{
}
