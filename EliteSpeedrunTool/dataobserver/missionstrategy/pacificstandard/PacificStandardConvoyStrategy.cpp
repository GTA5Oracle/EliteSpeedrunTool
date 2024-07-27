#include "PacificStandardConvoyStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PacificStandardConvoyStrategy::PacificStandardConvoyStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PacificStandardConvoyStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事：车队");
}

QSet<QString> PacificStandardConvoyStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString PacificStandardConvoyStrategy::id()
{
    return "PacificStandardConvoy";
}

const QSet<unsigned long long> PacificStandardConvoyStrategy::missionHash()
{
    return { MemoryUtil::hashPacificStandardConvoy };
}
