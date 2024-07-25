#include "Act3RescueAgent14Strategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act3RescueAgent14Strategy::Act3RescueAgent14Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act3RescueAgent14Strategy::getDisplayName()
{
    return tr("准备任务：营救 14 号探员");
}

QSet<QString> Act3RescueAgent14Strategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString Act3RescueAgent14Strategy::id()
{
    return "Act3RescueAgent14";
}

const QSet<unsigned long long> Act3RescueAgent14Strategy::missionHash()
{
    return { MemoryUtil::hashAct3RescueAgent14 };
}
