#include "SeriesAStealMethStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

SeriesAStealMethStrategy::SeriesAStealMethStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString SeriesAStealMethStrategy::getDisplayName()
{
    return tr("首轮募资：窃取冰毒");
}

QSet<QString> SeriesAStealMethStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString SeriesAStealMethStrategy::id()
{
    return "SeriesAStealMeth";
}

const QSet<unsigned long long> SeriesAStealMethStrategy::missionHash()
{
    return { MemoryUtil::hashSeriesAStealMeth };
}
