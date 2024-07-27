#include "SeriesAFundingStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

SeriesAFundingStrategy::SeriesAFundingStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString SeriesAFundingStrategy::getDisplayName()
{
    return tr("首轮募资");
}

QSet<QString> SeriesAFundingStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->killFetcher->id() };
}

QString SeriesAFundingStrategy::id()
{
    return "SeriesAFunding";
}

const QSet<unsigned long long> SeriesAFundingStrategy::missionHash()
{
    return { MemoryUtil::hashSeriesAFunding };
}
