#include "PacificStandardHackStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

PacificStandardHackStrategy::PacificStandardHackStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString PacificStandardHackStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事：破解");
}

QSet<QString> PacificStandardHackStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString PacificStandardHackStrategy::id()
{
    return "PacificStandardHack";
}

const QSet<unsigned long long> PacificStandardHackStrategy::missionHash()
{
    return { MemoryUtil::hashPacificStandardHack };
}
