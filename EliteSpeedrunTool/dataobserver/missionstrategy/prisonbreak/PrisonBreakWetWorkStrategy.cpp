#include "PrisonBreakWetWorkStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PrisonBreakWetWorkStrategy::PrisonBreakWetWorkStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PrisonBreakWetWorkStrategy::getDisplayName()
{
    return tr("越狱：行刺");
}

QSet<QString> PrisonBreakWetWorkStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString PrisonBreakWetWorkStrategy::id()
{
    return "PrisonBreakWetWork";
}

const QSet<unsigned long long> PrisonBreakWetWorkStrategy::missionHash()
{
    return { MemoryUtil::hashPrisonBreakWetWork };
}
