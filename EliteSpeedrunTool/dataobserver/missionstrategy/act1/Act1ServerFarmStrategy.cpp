#include "Act1ServerFarmStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act1ServerFarmStrategy::Act1ServerFarmStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act1ServerFarmStrategy::getDisplayName()
{
    return tr("准备任务：服务器群组");
}

QSet<QString> Act1ServerFarmStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString Act1ServerFarmStrategy::id()
{
    return "Act1ServerFarm";
}

const QSet<unsigned long long> Act1ServerFarmStrategy::missionHash()
{
    return { MemoryUtil::hashAct1ServerFarm };
}
