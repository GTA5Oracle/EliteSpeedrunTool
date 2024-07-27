#include "PacificStandardVansStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PacificStandardVansStrategy::PacificStandardVansStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PacificStandardVansStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事：厢型车");
}

QSet<QString> PacificStandardVansStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString PacificStandardVansStrategy::id()
{
    return "PacificStandardVans";
}

const QSet<unsigned long long> PacificStandardVansStrategy::missionHash()
{
    return { MemoryUtil::hashPacificStandardVans };
}
