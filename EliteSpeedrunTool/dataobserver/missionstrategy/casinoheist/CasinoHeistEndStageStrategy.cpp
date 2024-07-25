#include "CasinoHeistEndStageStrategy.h"
#include "MemoryUtil.h"

CasinoHeistEndStageStrategy::CasinoHeistEndStageStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString CasinoHeistEndStageStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫（结算前）");
}

QSet<QString> CasinoHeistEndStageStrategy::defaultDataFetchers()
{
    return {};
}

QString CasinoHeistEndStageStrategy::id()
{
    return "CasinoHeistEndStage";
}

const QSet<unsigned long long> CasinoHeistEndStageStrategy::missionHash()
{
    return { MemoryUtil::hashCasinoHeistEndStage };
}
