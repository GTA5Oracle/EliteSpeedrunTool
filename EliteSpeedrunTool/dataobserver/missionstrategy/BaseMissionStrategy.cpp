#include "BaseMissionStrategy.h"
#include "MissionStrategyUtil.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"
#include <QGraphicsDropShadowEffect>

BaseMissionStrategy::BaseMissionStrategy(QObject* parent)
    : QObject { parent }
{
    missionNameFont.setPointSize(14);
    labMissionName.setFont(missionNameFont);
}

BaseMissionStrategy::~BaseMissionStrategy()
{
}

void BaseMissionStrategy::setCurrentStrategy(bool isCurrent)
{
    this->isCurrent = isCurrent;
    const auto& dataFetchers = missionStrategyUtil->missionToDataFetchers()[id()];
    for (const QString& fetcher : dataFetchers) {
        dataFetcherUtil->dataFetcherMap[fetcher]->setShowing(isCurrent);
    }
    if (isCurrent) {
        if (!isLaterInited) {
            isLaterInited = true;
            connect(missionStrategyUtil, &MissionStrategyUtil::onMissionToDataFetchersChanged, this,
                [this](QString missionId, QSet<QString> oldFetchers, QSet<QString> newFetchers) {
                    if (missionId == id()) {
                        const auto& removedFetchers = oldFetchers - newFetchers;
                        for (const QString& fetcher : removedFetchers) {
                            dataFetcherUtil->dataFetcherMap[fetcher]->setShowing(false);
                        }
                        const auto& newerFetchers = newFetchers - oldFetchers;
                        for (const QString& fetcher : newerFetchers) {
                            dataFetcherUtil->dataFetcherMap[fetcher]->setShowing(true);
                        }
                    }
                });
            labMissionName.setText(getDisplayName());
        }
    }
}

bool BaseMissionStrategy::isCurrentStrategy()
{
    return isCurrent;
}

QList<QLabel*> BaseMissionStrategy::getDisplayLabels()
{
    QList<QLabel*> list = {};
    const auto& dataFetchers = missionStrategyUtil->missionToDataFetchers()[id()];
    for (const QString& fetcherId : dataFetchers) {
        list.push_back(dataFetcherUtil->dataFetcherMap[fetcherId]->getDisplayLabel());
    }
    return list;
}

QList<QLabel*> BaseMissionStrategy::getLabels()
{
    const auto& dataFetchers = missionStrategyUtil->missionToDataFetchers()[id()];
    QList<QLabel*> list = { &labMissionName };
    for (const QString& fetcherId : dataFetchers) {
        list.push_back(dataFetcherUtil->dataFetcherMap[fetcherId]->getLabel());
    }
    return list;
}

void BaseMissionStrategy::updateInfo()
{
    const auto& dataFetchers = missionStrategyUtil->missionToDataFetchers()[id()];
    for (const QString& fetcherId : dataFetchers) {
        dataFetcherUtil->dataFetcherMap[fetcherId]->fetchDataAndUpdateLabel();
    }
}

const QString BaseMissionStrategy::getUniqueDisplayName()
{
    return getDisplayName();
}

QSet<QString> BaseMissionStrategy::defaultDataFetchers()
{
    return {};
}
