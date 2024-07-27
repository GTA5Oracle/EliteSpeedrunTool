#include "PacificStandardSignalStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PacificStandardSignalStrategy::PacificStandardSignalStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PacificStandardSignalStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事：信号");
}

QSet<QString> PacificStandardSignalStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString PacificStandardSignalStrategy::id()
{
    return "PacificStandardSignal";
}

const QSet<unsigned long long> PacificStandardSignalStrategy::missionHash()
{
    return { MemoryUtil::hashPacificStandardSignal };
}
