#include "Act2RescueUlpStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act2RescueUlpStrategy::Act2RescueUlpStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act2RescueUlpStrategy::getDisplayName()
{
    return tr("准备任务：营救 ULP");
}

QSet<QString> Act2RescueUlpStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString Act2RescueUlpStrategy::id()
{
    return "Act2RescueUlp";
}

const QSet<unsigned long long> Act2RescueUlpStrategy::missionHash()
{
    return { MemoryUtil::hashAct2RescueUlp };
}
