#include "DataObserver.h"
#include "GlobalData.h"
#include "HttpServerUtil.h"

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
    connect(globalData, &GlobalData::missionDataUpdateIntervalChanged, this, [this]() {
        if (missionHashTimer) {
            missionHashTimer->setInterval(globalData->missionDataUpdateInterval());
        }
    });
    connect(globalData, &GlobalData::missionDataUpdateIntervalChanged, this, [this]() {
        if (timer) {
            timer->setInterval(globalData->missionDataUpdateInterval());
        }
    });
}

DataObserver::~DataObserver()
{
    destruct();
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
        missionStrategy->setCurrentStrategy(false);
    }
    if (!newMissionStrategy) {
        stopObserve();
        missionStrategy = nullptr;
        return;
    }
    missionStrategy = newMissionStrategy;
    newMissionStrategy->setCurrentStrategy(true);
    emit onMissionChanged();
    emit onLabelsAdded(newMissionStrategy->getLabels());
    emit onDisplayLabelsAdded(newMissionStrategy->getDisplayLabels());
}

void DataObserver::startObserve()
{
    if (isObserving) {
        return;
    }
    isObserving = true;
    missionHashTimer->start(globalData->missionDataUpdateInterval());
    timer->start(globalData->missionDataUpdateInterval());
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

// 析构整个DataObserver，这时会发出onXxxRemoved信号，让其他持有QLabel的控件取消持有
void DataObserver::destruct()
{
    stopObserve();
    if (destructed) {
        return;
    }
    destructed = true;
    if (missionStrategy) {
        emit onLabelsRemoved(missionStrategy->getLabels());
        emit onDisplayLabelsRemoved(missionStrategy->getDisplayLabels());
        missionStrategy->setCurrentStrategy(false);
        missionStrategy = nullptr;
    }
}

QList<QLabel*> DataObserver::getDisplayLabels()
{
    return missionStrategy ? missionStrategy->getDisplayLabels() : QList<QLabel*>();
}

void DataObserver::onTimeout()
{
    /* 当前有合适的任务策略 */
    if (missionStrategy) {

        /* 在任务中并且能够操控 */
        if (1 == memoryUtil->getLocalInt(MemoryUtil::localInMissionCanControl)) {
            missionStrategy->updateInfo();
        }

        // 发送最新信息
        auto labels = missionStrategy->getLabels();
        static QList<QString> missionDatas;
        missionDatas.clear();
        for (auto lab : labels) {
            missionDatas += lab->text();
        }
        HttpServerController::instance()->sendNewData(missionDatas.join("<br/>"));
    }
}

void DataObserver::onMissionHashTimeout()
{
    auto newMissionHash = missionHashFetcher.fetchData();
    if (globalData->debugMode()) {
        qDebug() << newMissionHash;
    }
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
