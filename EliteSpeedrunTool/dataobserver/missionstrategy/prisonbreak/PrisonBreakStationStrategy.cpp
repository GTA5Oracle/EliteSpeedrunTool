#include "PrisonBreakStationStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PrisonBreakStationStrategy::PrisonBreakStationStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PrisonBreakStationStrategy::getDisplayName()
{
    return tr("越狱：警察局");
}

QSet<QString> PrisonBreakStationStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->vehicleDamageFetcher->id(), dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString PrisonBreakStationStrategy::id()
{
    return "PrisonBreakStation";
}

const QSet<unsigned long long> PrisonBreakStationStrategy::missionHash()
{
    return { MemoryUtil::hashPrisonBreakStation };
}
