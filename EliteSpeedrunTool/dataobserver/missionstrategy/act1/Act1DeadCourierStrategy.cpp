#include "Act1DeadCourierStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act1DeadCourierStrategy::Act1DeadCourierStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act1DeadCourierStrategy::getDisplayName()
{
    return tr("准备任务：亡命速递");
}

QSet<QString> Act1DeadCourierStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString Act1DeadCourierStrategy::id()
{
    return "Act1DeadCourier";
}

const QSet<unsigned long long> Act1DeadCourierStrategy::missionHash()
{
    return { MemoryUtil::hashAct1DeadCourier };
}
