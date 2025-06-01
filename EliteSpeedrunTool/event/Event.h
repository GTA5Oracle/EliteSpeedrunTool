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

class CloseLegacyGameEvent : public Event {
    Q_OBJECT
};

class CloseEnhancedGameEvent : public Event {
    Q_OBJECT
};

class FirewallEvent : public Event {
    Q_OBJECT
public:
    FirewallEvent(bool enabled)
        : enabled { enabled } {};
    bool enabled = false;
};
