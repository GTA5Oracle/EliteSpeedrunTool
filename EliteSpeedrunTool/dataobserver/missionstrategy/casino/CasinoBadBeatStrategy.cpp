#include "CasinoBadBeatStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

CasinoBadBeatStrategy::CasinoBadBeatStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString CasinoBadBeatStrategy::getDisplayName()
{
    return tr("赌场 - 出奇制胜");
}

QSet<QString> CasinoBadBeatStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->killFetcher->id() };
}

QString CasinoBadBeatStrategy::id()
{
    return "CasinoBadBeat";
}

const QSet<unsigned long long> CasinoBadBeatStrategy::missionHash()
{
    return { MemoryUtil::hashCasinoBadBeat };
}
