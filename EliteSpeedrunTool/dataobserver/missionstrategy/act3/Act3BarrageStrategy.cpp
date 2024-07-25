#include "Act3BarrageStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act3BarrageStrategy::Act3BarrageStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act3BarrageStrategy::getDisplayName()
{
    return tr("准备任务：巴拉杰");
}

QSet<QString> Act3BarrageStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString Act3BarrageStrategy::id()
{
    return "Act3Barrage";
}

const QSet<unsigned long long> Act3BarrageStrategy::missionHash()
{
    return { MemoryUtil::hashAct3Barrage };
}
