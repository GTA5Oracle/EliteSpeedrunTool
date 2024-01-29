#include "CasinoHeistAggressiveStrategy.h"
#include "GlobalData.h"

CasinoHeistAggressiveStrategy::CasinoHeistAggressiveStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> CasinoHeistAggressiveStrategy::getDisplayLabels()
{
    return { &labDisplayHeadshot };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> CasinoHeistAggressiveStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]) };
}

QList<QLabel*> CasinoHeistAggressiveStrategy::getLabels()
{
    return { &labMissionName, &labHeadshot };
}

void CasinoHeistAggressiveStrategy::updateInfo()
{
    auto data = headshotFetcher.fetchData();
    updateHeadshotLabel(data);
}

const QString CasinoHeistAggressiveStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：气势汹汹");
}
