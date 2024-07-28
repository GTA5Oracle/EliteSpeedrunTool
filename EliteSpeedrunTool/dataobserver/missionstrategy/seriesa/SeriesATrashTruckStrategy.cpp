#include "SeriesATrashTruckStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

SeriesATrashTruckStrategy::SeriesATrashTruckStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString SeriesATrashTruckStrategy::getDisplayName()
{
    return tr("首轮募资：垃圾车");
}

QSet<QString> SeriesATrashTruckStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString SeriesATrashTruckStrategy::id()
{
    return "SeriesATrashTruck";
}

const QSet<unsigned long long> SeriesATrashTruckStrategy::missionHash()
{
    return { MemoryUtil::hashSeriesATrashTruck };
}
