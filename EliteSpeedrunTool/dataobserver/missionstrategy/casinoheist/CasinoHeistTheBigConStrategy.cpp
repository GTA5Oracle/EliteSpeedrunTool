#include "CasinoHeistTheBigConStrategy.h"
#include "MemoryUtil.h"

CasinoHeistTheBigConStrategy::CasinoHeistTheBigConStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString CasinoHeistTheBigConStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：兵不厌诈");
}

QSet<QString> CasinoHeistTheBigConStrategy::defaultDataFetchers()
{
    return {};
}

QString CasinoHeistTheBigConStrategy::id()
{
    return "CasinoHeistTheBigCon";
}

const QSet<unsigned long long> CasinoHeistTheBigConStrategy::missionHash()
{
    return {
        MemoryUtil::hashCasinoHeistTheBigCon1,
        MemoryUtil::hashCasinoHeistTheBigCon2,
        MemoryUtil::hashCasinoHeistTheBigCon3,
        MemoryUtil::hashCasinoHeistTheBigCon4,
    };
}
