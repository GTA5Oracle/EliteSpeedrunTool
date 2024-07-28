#include "PrisonBreakPlaneStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PrisonBreakPlaneStrategy::PrisonBreakPlaneStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PrisonBreakPlaneStrategy::getDisplayName()
{
    return tr("越狱：飞机");
}

QSet<QString> PrisonBreakPlaneStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString PrisonBreakPlaneStrategy::id()
{
    return "PrisonBreakPlane";
}

const QSet<unsigned long long> PrisonBreakPlaneStrategy::missionHash()
{
    return { MemoryUtil::hashPrisonBreakPlane };
}
