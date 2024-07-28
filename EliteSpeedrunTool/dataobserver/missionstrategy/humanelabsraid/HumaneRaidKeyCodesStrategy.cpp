#include "HumaneRaidKeyCodesStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

HumaneRaidKeyCodesStrategy::HumaneRaidKeyCodesStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString HumaneRaidKeyCodesStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室：关键密码");
}

QSet<QString> HumaneRaidKeyCodesStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString HumaneRaidKeyCodesStrategy::id()
{
    return "HumaneRaidKeyCodes";
}

const QSet<unsigned long long> HumaneRaidKeyCodesStrategy::missionHash()
{
    return {
        MemoryUtil::hashHumaneRaidKeyCodes1,
        MemoryUtil::hashHumaneRaidKeyCodes2,
    };
}
