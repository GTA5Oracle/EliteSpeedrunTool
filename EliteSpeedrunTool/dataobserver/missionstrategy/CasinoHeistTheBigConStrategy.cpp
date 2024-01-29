#include "CasinoHeistTheBigConStrategy.h"

CasinoHeistTheBigConStrategy::CasinoHeistTheBigConStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> CasinoHeistTheBigConStrategy::getDisplayLabels()
{
    return {};
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> CasinoHeistTheBigConStrategy::getDisplayLabelsAndItems()
{
    return {};
}

QList<QLabel*> CasinoHeistTheBigConStrategy::getLabels()
{
    return { &labMissionName };
}

void CasinoHeistTheBigConStrategy::updateInfo()
{
}

const QString CasinoHeistTheBigConStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：兵不厌诈");
}
