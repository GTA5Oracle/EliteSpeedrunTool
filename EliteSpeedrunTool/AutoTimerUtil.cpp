#include "AutoTimerUtil.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include "TimeUtil.h"
#include <QDateTime>
#include <chrono>

Q_GLOBAL_STATIC(AutoTimerUtil, autoTimerUtilInstance)

AutoTimerUtil::AutoTimerUtil()
{
    connect(timer, &QTimer::timeout, this, [this]() {
        timeOut();
    });
    timer->setTimerType(Qt::PreciseTimer);
}

AutoTimerUtil* AutoTimerUtil::instance()
{
    return autoTimerUtilInstance;
}

void AutoTimerUtil::start()
{
    if (timerRunning) {
        return;
    }
    timerRunning = true;
    MemoryUtil::initGlobalPtr();
    MemoryUtil::initMissionPtr();

    connect(globalData, &GlobalData::autoTimerUpdateIntervalChanged, this, [this]() {
        if (timer) {
            timer->setInterval(globalData->autoTimerUpdateInterval());
        }
    });
    timer->start(globalData->autoTimerUpdateInterval());
}

void AutoTimerUtil::stop()
{
    if (!timerRunning) {
        return;
    }
    timerRunning = false;
    stopAndReset(false);
}

void AutoTimerUtil::timeOut()
{
    gtaHandle = MemoryUtil::getProcessHandle(&pid);

    startTimerFlag = MemoryUtil::getLocalInt(MemoryUtil::local_Flag_initTimer);
    timeSummary = MemoryUtil::getGlobalUInt(2685249 + 6465);
    currentStateStartTime = MemoryUtil::getLocalLongLong(19728 + 985); // 开始时间ptr
    currentStateTime = MemoryUtil::getLocalInt(MemoryUtil::local_Time); // 时间ptr
    missionHash = MemoryUtil::getGlobalUInt(4718592 + 126144); // hash
    // qDebug() << missionHash << lastMissionHash;
    // 智障门有一瞬间hash变成0，解决方法是延时判断
    static long long hashReallyValueTime = 0;
    if (missionHash == 0 && lastMissionHash != 0) {
        if (!hashReallyValueTime) {
            hashReallyValueTime = getCurrentTimeStamp();
            waitingHash5s = true;
        }
    }
    if (waitingHash5s) {
        if (missionHash || getCurrentTimeStamp() - hashReallyValueTime > 5000) {
            hashReallyValueTime = 0;
            waitingHash5s = false;
        } else {
            // 延时判断hash
            missionHash = lastMissionHash;
        }
    }

    CloseHandle(gtaHandle);

    // qDebug() << MemoryUtil::globalPtr << state << missionHash << startTimerFlag << timeSummary << currentStateTime << currentStateStartTime;

    if (state == MissionState::Running && lastDoneState == MissionState::End) {
        flagTimeChangedTo1 = false;
        localTimeChangedToUnzero = false;
        missionHashChangedToZero = false;
    }

    if (startTimerFlag == 1 && lastStartTimerFlag == 0 && missionHash) {
        flagTimeChangedTo1 = true;
    }
    if (currentStateTime != 0 && lastCurrentStateTime == 0 && missionHash) {
        localTimeChangedToUnzero = true;
    }
    if (missionHash == 0 && lastMissionHash != 0) {
        missionHashChangedToZero = true;
    }

    onTimerCallback();

    if (state == MissionState::Running) {
        if (lastDoneState != state) {
            deltaLocalServerTime = getCurrentTimeStamp() - MemoryUtil::getLocalLongLong(19728 + 985); // 开始时间ptr
        }
        auto deltaTime = getCurrentTimeStamp() - currentStateStartTime - deltaLocalServerTime;
        // qDebug() << deltaTime << getCurrentTimeStamp() << currentStateStartTime << deltaLocalServerTime;
        deltaTime += timeSummary;
        time = deltaTime;
        sendUpdateTimeSignal(time);
        lastDoneState = state;
    } else if (lastDoneState != state && lastDoneState != MissionState::None) {
        // 下面的动作每次访问时相同的只能做一次
        if (state == MissionState::Pause) {
            time = timeSummary + currentStateTime;
            sendUpdateTimeSignal(time);
        }
        lastDoneState = state;
    }

    lastStartTimerFlag = startTimerFlag;
    lastCurrentStateTime = currentStateTime;
    lastCurrentStateStartTime = currentStateStartTime;
    lastMissionHash = missionHash;
}

long long AutoTimerUtil::getCurrentTimeStamp()
{
    return QDateTime::currentDateTimeUtc()
        .toTimeZone(QTimeZone("UTC+3"))
        .time()
        .msecsSinceStartOfDay();
}

void AutoTimerUtil::stopAndReset(bool resetTime)
{
    if (timer->isActive()) {
        timer->stop();
        emit stopped();
    }
    if (resetTime) {
        time = 0;
        // 不要用sendUpdateTimeSignal，因为现在是重置状态，time都是0，不会+10s
        emit updateTime(0);
    }

    state = lastDoneState = MissionState::None;

    startTimerFlag = timeSummary = currentStateTime
        = currentStateStartTime = missionHash = 0;

    startDisplayedTimer = false;

    deltaLocalServerTime = 0;

    lastStartTimerFlag = lastTimeSummary = lastCurrentStateTime
        = lastCurrentStateStartTime = lastMissionHash = 0;

    flagTimeChangedTo1 = localTimeChangedToUnzero
        = missionHashChangedToZero = waitingHash5s = false;
}

void AutoTimerUtil::onTimerCallback()
{
    // 监控flag_time   0 >>1   如果变1  开始计时
    if (flagTimeChangedTo1) {
        startDisplayedTimer = true;
        state = MissionState::Running;
        flagTimeChangedTo1 = false;
    }
    if (missionHashChangedToZero) {
        // 结算hash会变0
        missionHashChangedToZero = false;
        state = MissionState::End;
        return;
    }
    if (!startDisplayedTimer) {
        return;
    }
    // 监控local_time 如果0 >>!0  停止计时 记录当前值
    if (localTimeChangedToUnzero) {
        startDisplayedTimer = false;
        localTimeChangedToUnzero = false;
        state = MissionState::Pause;
    }
}

void AutoTimerUtil::sendUpdateTimeSignal(unsigned long long data)
{
    if (missionHash != 979654579) {
        // 除了越狱之外的任务默认+10s
        emit updateTime(data + 10000);
    } else {
        emit updateTime(data);
    }
}
