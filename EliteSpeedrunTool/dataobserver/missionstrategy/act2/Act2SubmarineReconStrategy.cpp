#include "Act2SubmarineReconStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act2SubmarineReconStrategy::Act2SubmarineReconStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act2SubmarineReconStrategy::getDisplayName()
{
    return tr("准备任务：潜水艇侦察");
}

QSet<QString> Act2SubmarineReconStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString Act2SubmarineReconStrategy::id()
{
    return "Act2SubmarineRecon";
}

const QSet<unsigned long long> Act2SubmarineReconStrategy::missionHash()
{
    return { MemoryUtil::hashAct2SubmarineRecon };
}
