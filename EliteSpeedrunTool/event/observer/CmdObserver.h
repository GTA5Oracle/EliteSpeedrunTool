#pragma once

#include "../EventBus.h"

#include <QProcess>

class CmdObserver : public EventObserver {
    Q_OBJECT
public:
    explicit CmdObserver(QObject* parent = nullptr);
    void onEvent(Event* event);

private:
    QProcess* process = new QProcess(this);
};
