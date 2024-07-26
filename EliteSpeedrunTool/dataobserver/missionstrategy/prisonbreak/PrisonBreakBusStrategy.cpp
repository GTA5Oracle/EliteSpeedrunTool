#include "PrisonBreakBusStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PrisonBreakBusStrategy::PrisonBreakBusStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PrisonBreakBusStrategy::getDisplayName()
{
    return tr("越狱：巴士");
}

QSet<QString> PrisonBreakBusStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString PrisonBreakBusStrategy::id()
{
    return "PrisonBreakBus";
}

const QSet<unsigned long long> PrisonBreakBusStrategy::missionHash()
{
    return { MemoryUtil::hashPrisonBreakBus };
}
