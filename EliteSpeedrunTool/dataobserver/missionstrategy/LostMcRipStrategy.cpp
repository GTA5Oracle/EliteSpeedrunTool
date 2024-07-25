#include "LostMcRipStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

LostMcRipStrategy::LostMcRipStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString LostMcRipStrategy::getDisplayName()
{
    return tr("安息吧！失落摩托帮");
}

QSet<QString> LostMcRipStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id(), dataFetcherUtil->killFetcher->id() };
}

QString LostMcRipStrategy::id()
{
    return "LostMcRip";
}

const QSet<unsigned long long> LostMcRipStrategy::missionHash()
{
    return { MemoryUtil::hashLostMcRip };
}
