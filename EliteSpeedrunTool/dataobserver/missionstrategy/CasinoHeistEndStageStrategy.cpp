#include "CasinoHeistEndStageStrategy.h"

CasinoHeistEndStageStrategy::CasinoHeistEndStageStrategy(QObject* parent)
    : CasinoHeistAggressiveStrategy { parent }
{
}

const QString CasinoHeistEndStageStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫");
}
