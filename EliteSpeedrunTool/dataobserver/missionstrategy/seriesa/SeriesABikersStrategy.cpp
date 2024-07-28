#include "SeriesABikersStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

SeriesABikersStrategy::SeriesABikersStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString SeriesABikersStrategy::getDisplayName()
{
    return tr("首轮募资：摩托车手");
}

QSet<QString> SeriesABikersStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString SeriesABikersStrategy::id()
{
    return "SeriesABikers";
}

const QSet<unsigned long long> SeriesABikersStrategy::missionHash()
{
    return { MemoryUtil::hashSeriesABikers };
}
