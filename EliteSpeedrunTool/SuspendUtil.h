#pragma once

#include <QObject>
#include <windows.h>

#define suspendUtil (SuspendUtil::instance())

class SuspendUtil : public QObject {
    Q_OBJECT
public:
    explicit SuspendUtil(QObject* parent = nullptr);

    static SuspendUtil* instance();

    bool isEulaAccepted();

    void acceptEula();

    void suspendProcess();

    void resumeProcess();

    void suspendAndResumeProcess(long long durationMs, std::function<void(void)> onResumed);

signals:
    void onAcceptedChanged();
};
