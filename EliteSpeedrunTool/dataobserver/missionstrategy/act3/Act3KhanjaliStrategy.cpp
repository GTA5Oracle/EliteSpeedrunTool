#include "Act3KhanjaliStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act3KhanjaliStrategy::Act3KhanjaliStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act3KhanjaliStrategy::getDisplayName()
{
    return tr("准备任务：可汗贾利");
}

QSet<QString> Act3KhanjaliStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString Act3KhanjaliStrategy::id()
{
    return "Act3Khanjali";
}

const QSet<unsigned long long> Act3KhanjaliStrategy::missionHash()
{
    return { MemoryUtil::hashAct3Khanjali };
}
