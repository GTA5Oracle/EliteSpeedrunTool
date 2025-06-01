#pragma once

#include <QObject>
#include <QSet>

#define eventBus (EventBus::instance())

class Event : public QObject {
    Q_OBJECT
public:
    explicit Event(QObject* parent = nullptr);
};

class EventObserver : public QObject {
public:
    explicit EventObserver(QObject* parent = nullptr);
    virtual void onEvent(Event* event) = 0;
};

class EventBus : public QObject {
    Q_OBJECT
public:
    explicit EventBus(QObject* parent = nullptr);
    ~EventBus();

    static EventBus* instance();

    void send(Event* event);

    void addObserver(EventObserver* observer);
    void removeObserver(EventObserver* observer);

private:
    QSet<EventObserver*> observers;
signals:
    void onEvent(Event* event);
};
