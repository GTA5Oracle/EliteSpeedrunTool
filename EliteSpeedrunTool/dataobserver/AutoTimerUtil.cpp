#include "dataobserver/AutoTimerUtil.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include <QDateTime>

Q_GLOBAL_STATIC(AutoTimerUtil, autoTimerUtilInstance)

AutoTimerUtil::AutoTimerUtil()
{
    connect(timer, &QTimer::timeout, this, [this]() {
        timeOut();
    });
    timer->setTimerType(Qt::PreciseTimer);

    connect(memoryUtil, &MemoryUtil::onMissionPtrChanged, this, [this]() {
        stopAndReset(true);
        start();
    });
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
    gtaHandle = memoryUtil->getProcessHandle(&pid);

    missionHash = memoryUtil->getGlobalUInt(MemoryUtil::globalMissionHash); // hash
    if (globalData->debugMode()) {
        qDebug() << missionHash << lastMissionHash;
    }
    if (missionHash == MemoryUtil::hashPrisonBreak) {
        startTimerFlag = memoryUtil->getLocalInt(MemoryUtil::localFlagInitPrisonBreakTimer);
        summaryTime = memoryUtil->getGlobalUInt(MemoryUtil::globalPrisonBreakSummaryTime);
        currentStateStartTime = memoryUtil->getLocalULongLong(MemoryUtil::localInitPrisonBreakTimestamp); // 开始时间ptr
        currentStateTime = memoryUtil->getLocalUInt(MemoryUtil::localPrisonBreakTime); // 时间ptr
    } else {
        startTimerFlag = memoryUtil->getLocalInt(MemoryUtil::localFlagInitTimer);
        summaryTime = memoryUtil->getGlobalUInt(MemoryUtil::globalSummaryTime);
        currentStateStartTime = memoryUtil->getLocalULongLong(MemoryUtil::localInitTimestamp); // 开始时间ptr
        currentStateTime = memoryUtil->getLocalUInt(MemoryUtil::localTime); // 时间ptr
    }
    inMissionCanControl = memoryUtil->getLocalUInt(MemoryUtil::localInMissionCanControl); // 是不是在任务中并且能够操控
    // 智障门有一瞬间hash变成0，解决方法是延时判断
    static unsigned long long hashReallyValueTime = 0;
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

    if (globalData->debugMode()) {
        qDebug() << memoryUtil->globalPtr << state << missionHash << startTimerFlag << summaryTime << currentStateTime << currentStateStartTime << inMissionCanControl;
    }

    if (state == MissionState::Running && lastDoneState == MissionState::End) {
        flagTimeChangedTo1 = false;
        localTimeChangedToUnzero = false;
        missionHashChangedToZero = false;

        deltaLocalServerTime = 0;

        lastStartTimerFlag = lastTimeSummary = lastCurrentStateTime
            = lastCurrentStateStartTime = lastMissionHash = 0;
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
            deltaLocalServerTime = getCurrentTimeStamp()
                - memoryUtil->getLocalULongLong(
                    missionHash == MemoryUtil::hashPrisonBreak
                        ? MemoryUtil::localInitPrisonBreakTimestamp
                        : MemoryUtil::localInitTimestamp); // 开始时间ptr
        }
        unsigned long long deltaTime = getCurrentTimeStamp()
            - currentStateStartTime
            - deltaLocalServerTime;
        if (globalData->debugMode()) {
            qDebug() << deltaTime
                     << getCurrentTimeStamp()
                     << currentStateStartTime
                     << deltaLocalServerTime;
        }
        deltaTime += summaryTime;
        time = deltaTime;
        sendUpdateTimeSignal(time);
        if (lastDoneState != state) {
            emit timerStarted(getRealTime(time));
        }
        lastDoneState = state;
    } else if (lastDoneState != state && lastDoneState != MissionState::None) {
        // 下面的动作每次访问时相同的只能做一次
        if (state == MissionState::Pause) {
            time = summaryTime + currentStateTime;
            emit timerPaused(sendUpdateTimeSignal(time));
        }
        lastDoneState = state;
    }

    lastStartTimerFlag = startTimerFlag;
    lastCurrentStateTime = currentStateTime;
    lastCurrentStateStartTime = currentStateStartTime;
    lastMissionHash = missionHash;
}

unsigned long long AutoTimerUtil::getCurrentTimeStamp()
{
    return static_cast<unsigned long long>(
        QDateTime::currentDateTimeUtc()
            .toTimeZone(QTimeZone("UTC+3"))
            .time()
            .msecsSinceStartOfDay());
}

void AutoTimerUtil::stopAndReset(bool resetTime)
{
    if (timer->isActive()) {
        timer->stop();
        emit stopped();
        emit timerPaused(getRealTime(time));
    }
    if (resetTime) {
        time = 0;
        // 不要用sendUpdateTimeSignal，因为现在是重置状态，time都是0，不会+10s
        emit updateTime(0);
        emit timerPaused(0);
    }

    state = lastDoneState = MissionState::None;

    startTimerFlag = summaryTime = currentStateTime
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
    // 在任务中，并且 监控flag_time   0 >>1   如果变1  开始计时
    if (inMissionCanControl && flagTimeChangedTo1) {
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

unsigned long long AutoTimerUtil::getRealTime(unsigned long long data)
{
    if (missionHash != MemoryUtil::hashPrisonBreak) {
        // 除了越狱之外的任务默认+10s
        return data + 10000;
    } else {
        return data;
    }
}

unsigned long long AutoTimerUtil::sendUpdateTimeSignal(unsigned long long data)
{
    auto realTime = getRealTime(data);
    emit updateTime(realTime);
    return realTime;
}
