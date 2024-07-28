#include "HumaneRaidValkyrieStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

HumaneRaidValkyrieStrategy::HumaneRaidValkyrieStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString HumaneRaidValkyrieStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室：女武神");
}

QSet<QString> HumaneRaidValkyrieStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString HumaneRaidValkyrieStrategy::id()
{
    return "HumaneRaidValkyrie";
}

const QSet<unsigned long long> HumaneRaidValkyrieStrategy::missionHash()
{
    return { MemoryUtil::hashHumaneRaidValkyrie };
}
