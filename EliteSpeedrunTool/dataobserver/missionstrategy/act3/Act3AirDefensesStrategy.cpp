#include "Act3AirDefensesStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act3AirDefensesStrategy::Act3AirDefensesStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act3AirDefensesStrategy::getDisplayName()
{
    return tr("准备任务：空中防御");
}

QSet<QString> Act3AirDefensesStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString Act3AirDefensesStrategy::id()
{
    return "Act3AirDefenses";
}

const QSet<unsigned long long> Act3AirDefensesStrategy::missionHash()
{
    return { MemoryUtil::hashAct3AirDefenses };
}
