#pragma once

#include "EventBus.h"

class StartEvent : public Event {
    Q_OBJECT
};

class QuitEvent : public Event {
    Q_OBJECT
};

class CloseGameEvent : public Event {
    Q_OBJECT
};
