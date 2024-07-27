#include "SeriesAWeedStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

SeriesAWeedStrategy::SeriesAWeedStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString SeriesAWeedStrategy::getDisplayName()
{
    return tr("首轮募资：大麻");
}

QSet<QString> SeriesAWeedStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString SeriesAWeedStrategy::id()
{
    return "SeriesAWeed";
}

const QSet<unsigned long long> SeriesAWeedStrategy::missionHash()
{
    return { MemoryUtil::hashSeriesAWeed };
}
