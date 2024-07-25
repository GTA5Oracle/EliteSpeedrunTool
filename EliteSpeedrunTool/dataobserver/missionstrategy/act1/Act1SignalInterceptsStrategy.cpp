#include "Act1SignalInterceptsStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

Act1SignalInterceptsStrategy::Act1SignalInterceptsStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString Act1SignalInterceptsStrategy::getDisplayName()
{
    return tr("准备任务：拦截信号");
}

QSet<QString> Act1SignalInterceptsStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id() };
}

QString Act1SignalInterceptsStrategy::id()
{
    return "Act1SignalIntercepts";
}

const QSet<unsigned long long> Act1SignalInterceptsStrategy::missionHash()
{
    return { MemoryUtil::hashAct1SignalIntercepts };
}
