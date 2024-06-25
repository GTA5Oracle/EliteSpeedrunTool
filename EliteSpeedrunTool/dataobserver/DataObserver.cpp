#include "DataObserver.h"
#include "GlobalData.h"
#include "HttpServerUtil.h"

Q_GLOBAL_STATIC(DataObserver, dataObserverInstance)

DataObserver::DataObserver(QObject* parent)
    : QObject { parent }
{
    connect(timer, &QTimer::timeout, this, &DataObserver::onTimeout);
    connect(missionHashTimer, &QTimer::timeout, this, &DataObserver::onMissionHashTimeout);

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
    if (newMissionStrategy == missionStrategy) {
        return;
    }
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
    // 在上面添加了Label后（Label有parent后）更新第一次的数据
    newMissionStrategy->resetLabelData();
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
        /* 当前在支持的任务中 并且 能够操控 */
        if (missionStrategy != emptyStrategy
            && 1 == memoryUtil->getLocalInt(MemoryUtil::localInMissionCanControl)) {
            missionStrategy->updateInfo();
        }

        // 发送最新信息
        if (HttpServerController::instance()->isStarted()) {
            auto labels = missionStrategy->getLabels();
            static QList<QString> missionDatas;
            missionDatas.clear();
            for (auto lab : labels) {
                missionDatas += lab->text();
            }
            HttpServerController::instance()->sendNewData(missionDatas.join("<br/>"));
        }
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
        // 跟上次的MissionStrategy相同，直接跳过
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
