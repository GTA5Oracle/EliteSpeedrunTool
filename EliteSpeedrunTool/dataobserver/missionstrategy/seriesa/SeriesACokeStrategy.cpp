#include "SeriesACokeStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

SeriesACokeStrategy::SeriesACokeStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString SeriesACokeStrategy::getDisplayName()
{
    return tr("首轮募资：可卡因");
}

QSet<QString> SeriesACokeStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString SeriesACokeStrategy::id()
{
    return "SeriesACoke";
}

const QSet<unsigned long long> SeriesACokeStrategy::missionHash()
{
    return { MemoryUtil::hashSeriesACoke };
}
