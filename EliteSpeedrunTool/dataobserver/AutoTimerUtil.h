#pragma once

#include "AutoCapture.h"
#include "MemoryUtil.h"
#include <QDateTime>
#include <QTimeZone>
#include <QTimer>
#define autoTimerUtil (AutoTimerUtil::instance())

enum MissionState {
    None,
    Running,
    Pause,
    End,
};

class AutoTimerUtil : public QObject {
    Q_OBJECT
public:
    AutoTimerUtil();

    static AutoTimerUtil* instance();

    void start();
    void stop();
    void timeOut();
    long long getCurrentTimeStamp();
    void stopAndReset(bool resetTime = true);

private:
    void onTimerCallback();
    void sendUpdateTimeSignal(unsigned long long data);

signals:
    void stopped();
    void updateTime(unsigned long long data);

private:
    bool timerRunning = false;

    unsigned long long time = 0;
    QTimer* timer = new QTimer(this);
    DWORD pid = 0;
    HANDLE gtaHandle = nullptr;

    MissionState state = MissionState::None;
    MissionState lastDoneState = MissionState::None;

    int startTimerFlag = 0;
    unsigned int summaryTime = 0;
    unsigned int currentStateTime = 0;
    unsigned long long currentStateStartTime = 0;
    unsigned long long missionHash = 0;

    bool startDisplayedTimer = false;
    DWORD64 deltaLocalServerTime = 0;

    int lastStartTimerFlag = 0;
    unsigned int lastTimeSummary = 0;
    unsigned int lastCurrentStateTime = 0;
    unsigned long long lastCurrentStateStartTime = 0;
    unsigned long long lastMissionHash = 0;

    bool flagTimeChangedTo1 = false;
    bool localTimeChangedToUnzero = false;
    bool missionHashChangedToZero = false;
    bool waitingHash5s = false;
};
