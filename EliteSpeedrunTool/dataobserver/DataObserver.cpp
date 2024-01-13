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
    isObserving = true;
    missionHashTimer->start(50);
    timer->start(50);
    emit onStartObserve();
}

void DataObserver::stopObserve()
{
    if (!isObserving) {
        return;
    }
    isObserving = false;
    missionHashTimer->stop();
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
    qDebug() << newMissionHash;
    // if (missionStrategy) {
    //     qDebug() << missionStrategy->getLabels();
    // }
    // 切换不同的任务策略
    if (missionStrategyMap.contains(newMissionHash)) {
        auto newMissionStrategy = missionStrategyMap[newMissionHash];
        if (missionStrategy != newMissionStrategy) {
            lastMissionHash = newMissionHash;
            setMissionStrategy(newMissionStrategy);
        }
    } else {
        if (missionStrategy != emptyStrategy) {
            lastMissionHash = newMissionHash;
            setMissionStrategy(emptyStrategy);
        }
    }
}
