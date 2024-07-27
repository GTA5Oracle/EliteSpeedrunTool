#include "PacificStandardBikesStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PacificStandardBikesStrategy::PacificStandardBikesStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PacificStandardBikesStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事：摩托车");
}

QSet<QString> PacificStandardBikesStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString PacificStandardBikesStrategy::id()
{
    return "PacificStandardBikes";
}

const QSet<unsigned long long> PacificStandardBikesStrategy::missionHash()
{
    return { MemoryUtil::hashPacificStandardBikes };
}
