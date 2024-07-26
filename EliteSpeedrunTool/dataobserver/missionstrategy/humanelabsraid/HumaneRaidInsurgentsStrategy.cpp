#include "HumaneRaidInsurgentsStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

HumaneRaidInsurgentsStrategy::HumaneRaidInsurgentsStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString HumaneRaidInsurgentsStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室：叛乱分子");
}

QSet<QString> HumaneRaidInsurgentsStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
        dataFetcherUtil->vehicleDamageFetcher->id(),
    };
}

QString HumaneRaidInsurgentsStrategy::id()
{
    return "HumaneRaidInsurgents";
}

const QSet<unsigned long long> HumaneRaidInsurgentsStrategy::missionHash()
{
    return { MemoryUtil::hashHumaneRaidInsurgents };
}
