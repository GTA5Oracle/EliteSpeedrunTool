#pragma once

#include <QDateTime>
#include <QTimeZone>
#include <QTimer>
#include <windows.h>
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
    unsigned long long getCurrentTimeStamp();
    void stopAndReset(bool resetTime = true);

private:
    void onTimerCallback();
    unsigned long long getRealTime(unsigned long long data);
    unsigned long long sendUpdateTimeSignal(unsigned long long data);

signals:
    void stopped();
    void updateTime(unsigned long long data);
    void timerStarted(unsigned long long data);
    void timerPaused(unsigned long long data);

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
    unsigned int missionHash = 0;
    unsigned int inMissionCanControl = 0;

    bool startDisplayedTimer = false;
    unsigned long long deltaLocalServerTime = 0;

    int lastStartTimerFlag = 0;
    unsigned int lastTimeSummary = 0;
    unsigned int lastCurrentStateTime = 0;
    unsigned long long lastCurrentStateStartTime = 0;
    unsigned int lastMissionHash = 0;

    bool flagTimeChangedTo1 = false;
    bool localTimeChangedToUnzero = false;
    bool missionHashChangedToZero = false;
    bool waitingHash5s = false;
};
