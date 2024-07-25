#include "Act2SalvageHardDrivesStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act2SalvageHardDrivesStrategy::Act2SalvageHardDrivesStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act2SalvageHardDrivesStrategy::getDisplayName()
{
    return tr("准备任务：抢救硬盘");
}

QSet<QString> Act2SalvageHardDrivesStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString Act2SalvageHardDrivesStrategy::id()
{
    return "Act2SalvageHardDrives";
}

const QSet<unsigned long long> Act2SalvageHardDrivesStrategy::missionHash()
{
    return { MemoryUtil::hashAct2SalvageHardDrives };
}
