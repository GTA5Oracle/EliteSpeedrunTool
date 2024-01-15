#include "CasinoHeistTheBigConStrategy.h"

CasinoHeistTheBigConStrategy::CasinoHeistTheBigConStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
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
    return QList<QLabel*>() << &labMissionName;
}

void CasinoHeistTheBigConStrategy::updateInfo()
{
}

const QString CasinoHeistTheBigConStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：兵不厌诈");
}
