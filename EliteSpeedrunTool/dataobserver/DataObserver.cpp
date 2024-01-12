#include "DataObserver.h"

Q_GLOBAL_STATIC(DataObserver, dataObserverInstance)

DataObserver::DataObserver(QObject* parent)
    : QObject { parent }
{
    connect(timer, &QTimer::timeout, this, [this]() {
        onTimeout();
    });
    connect(missionHashTimer, &QTimer::timeout, this, [this]() {
        onMissionHashTimeout();
    });
}

DataObserver* DataObserver::instance()
{
    return dataObserverInstance;
}

BaseMissionStrategy* DataObserver::getMissionStrategy() const
{
    return missionStrategy;
}

void DataObserver::setMissionStrategy(BaseMissionStrategy* newMissionStrategy)
{
    if (missionStrategy) {
        emit onLabelsRemoved(missionStrategy->getLabels());
        emit onDisplayLabelsRemoved(missionStrategy->getDisplayLabels());
        missionStrategy->remove();
    }
    if (!newMissionStrategy) {
        stopObserve();
        missionStrategy = nullptr;
        return;
    }
    missionStrategy = newMissionStrategy;
    emit onLabelsAdded(newMissionStrategy->getLabels());
    emit onDisplayLabelsAdded(newMissionStrategy->getDisplayLabels());
}

void DataObserver::startObserve()
{
    if (isObserving) {
        return;
    }
    setMissionStrategy(new DoomsdayAct3Strategy());
    isObserving = true;
    timer->start(50);
    emit onStartObserve();
}

void DataObserver::stopObserve()
{
    if (!isObserving) {
        return;
    }
    isObserving = false;
    timer->stop();
    emit onStopObserve();
}

void DataObserver::onTimeout()
{
    if (missionStrategy) {
        missionStrategy->updateInfo();
    }
}

void DataObserver::onMissionHashTimeout()
{
    auto newMissionHash = missionHashFetcher.fetchData();
    if (lastMissionHash != newMissionHash) {
        lastMissionHash = newMissionHash;
        // 切换不同的任务策略
        if (missionStrategyMap.contains(newMissionHash)) {
            auto newMissionStrategy = missionStrategyMap[newMissionHash];
            setMissionStrategy(newMissionStrategy.missionStrategy);
        }
    }
}
