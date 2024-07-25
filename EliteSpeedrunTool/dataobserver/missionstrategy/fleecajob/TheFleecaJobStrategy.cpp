#include "TheFleecaJobStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

TheFleecaJobStrategy::TheFleecaJobStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString TheFleecaJobStrategy::getDisplayName()
{
    return tr("全福银行差事");
}

QSet<QString> TheFleecaJobStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->vehicleDamageFetcher->id() };
}

QString TheFleecaJobStrategy::id()
{
    return "TheFleecaJob";
}

const QSet<unsigned long long> TheFleecaJobStrategy::missionHash()
{
    return { MemoryUtil::hashTheFleecaJob };
}
