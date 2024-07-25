#include "CasinoHeistSilentSneakyStrategy.h"
#include "MemoryUtil.h"

CasinoHeistSilentSneakyStrategy::CasinoHeistSilentSneakyStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString CasinoHeistSilentSneakyStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：隐迹潜踪");
}

QSet<QString> CasinoHeistSilentSneakyStrategy::defaultDataFetchers()
{
    return {};
}

QString CasinoHeistSilentSneakyStrategy::id()
{
    return "CasinoHeistSilentSneaky";
}

const QSet<unsigned long long> CasinoHeistSilentSneakyStrategy::missionHash()
{
    return {
        MemoryUtil::hashCasinoHeistSilentSneaky1,
        MemoryUtil::hashCasinoHeistSilentSneaky2,
        MemoryUtil::hashCasinoHeistSilentSneaky3,
        MemoryUtil::hashCasinoHeistSilentSneaky4,
    };
}
