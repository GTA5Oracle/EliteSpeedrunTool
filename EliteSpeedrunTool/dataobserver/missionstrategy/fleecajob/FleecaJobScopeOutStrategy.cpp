#include "FleecaJobScopeOutStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

FleecaJobScopeOutStrategy::FleecaJobScopeOutStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString FleecaJobScopeOutStrategy::getDisplayName()
{
    return tr("全福银行差事：探查");
}

QSet<QString> FleecaJobScopeOutStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->vehicleDamageFetcher->id() };
}

QString FleecaJobScopeOutStrategy::id()
{
    return "FleecaJobScopeOut";
}

const QSet<unsigned long long> FleecaJobScopeOutStrategy::missionHash()
{
    return { MemoryUtil::hashFleecaJobScopeOut };
}
