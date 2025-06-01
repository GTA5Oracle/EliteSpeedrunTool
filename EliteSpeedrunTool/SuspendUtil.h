#pragma once

#include <QObject>
#include <QTimer>
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

    void suspendAndResumeProcess(long long durationMs);

private:
    QTimer timer;
    bool isSuspending = false;

    void onTimeout();

signals:
    void onAcceptedChanged();
    void onResume();
};
