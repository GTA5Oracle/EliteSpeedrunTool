#include "Act3EscortUlpStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act3EscortUlpStrategy::Act3EscortUlpStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act3EscortUlpStrategy::getDisplayName()
{
    return tr("准备任务：护送 ULP");
}

QSet<QString> Act3EscortUlpStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString Act3EscortUlpStrategy::id()
{
    return "Act3EscortUlp";
}

const QSet<unsigned long long> Act3EscortUlpStrategy::missionHash()
{
    return { MemoryUtil::hashAct3EscortUlp };
}
