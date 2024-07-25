#include "CasinoHeistAggressiveStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

CasinoHeistAggressiveStrategy::CasinoHeistAggressiveStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString CasinoHeistAggressiveStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：气势汹汹");
}

QSet<QString> CasinoHeistAggressiveStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id() };
}

QString CasinoHeistAggressiveStrategy::id()
{
    return "CasinoHeistAggressive";
}

const QSet<unsigned long long> CasinoHeistAggressiveStrategy::missionHash()
{
    return {
        MemoryUtil::hashCasinoHeistAggressive1,
        MemoryUtil::hashCasinoHeistAggressive2,
        MemoryUtil::hashCasinoHeistAggressive3,
        MemoryUtil::hashCasinoHeistAggressive4,
        MemoryUtil::hashCasinoHeistAggressive5,
    };
}
