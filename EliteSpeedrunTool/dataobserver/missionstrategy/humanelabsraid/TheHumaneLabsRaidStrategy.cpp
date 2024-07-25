#include "TheHumaneLabsRaidStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

TheHumaneLabsRaidStrategy::TheHumaneLabsRaidStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString TheHumaneLabsRaidStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室");
}

QSet<QString> TheHumaneLabsRaidStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->vehicleDamageFetcher->id() };
}

QString TheHumaneLabsRaidStrategy::id()
{
    return "TheHumaneLabsRaid";
}

const QSet<unsigned long long> TheHumaneLabsRaidStrategy::missionHash()
{
    return { MemoryUtil::hashTheHumaneLabsRaid };
}
