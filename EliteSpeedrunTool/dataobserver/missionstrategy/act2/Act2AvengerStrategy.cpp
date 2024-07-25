#include "Act2AvengerStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act2AvengerStrategy::Act2AvengerStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act2AvengerStrategy::getDisplayName()
{
    return tr("准备任务：复仇者");
}

QSet<QString> Act2AvengerStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString Act2AvengerStrategy::id()
{
    return "Act2Avenger";
}

const QSet<unsigned long long> Act2AvengerStrategy::missionHash()
{
    return { MemoryUtil::hashAct2Avenger };
}
