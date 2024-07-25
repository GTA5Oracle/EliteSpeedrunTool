#include "FleecaJobKurumaStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

FleecaJobKurumaStrategy::FleecaJobKurumaStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString FleecaJobKurumaStrategy::getDisplayName()
{
    return tr("全福银行差事：骷髅马");
}

QSet<QString> FleecaJobKurumaStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
        dataFetcherUtil->vehicleDamageFetcher->id() };
}

QString FleecaJobKurumaStrategy::id()
{
    return "FleecaJobKuruma";
}

const QSet<unsigned long long> FleecaJobKurumaStrategy::missionHash()
{
    return { MemoryUtil::hashFleecaJobKuruma };
}
