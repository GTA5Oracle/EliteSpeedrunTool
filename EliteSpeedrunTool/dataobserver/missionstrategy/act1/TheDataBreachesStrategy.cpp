#include "TheDataBreachesStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

TheDataBreachesStrategy::TheDataBreachesStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString TheDataBreachesStrategy::getDisplayName()
{
    return tr("数据泄露");
}

QSet<QString> TheDataBreachesStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->killFetcher->id() };
}

QString TheDataBreachesStrategy::id()
{
    return "TheDataBreaches";
}

const QSet<unsigned long long> TheDataBreachesStrategy::missionHash()
{
    return { MemoryUtil::hashTheDataBreaches };
}
