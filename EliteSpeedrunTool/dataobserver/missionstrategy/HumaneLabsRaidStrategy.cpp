#include "HumaneLabsRaidStrategy.h"
#include "GlobalData.h"

HumaneLabsRaidStrategy::HumaneLabsRaidStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> HumaneLabsRaidStrategy::getDisplayLabels()
{
    return { &labDisplayPlaneDamage };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> HumaneLabsRaidStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayPlaneDamage, displayInfoSubFunctions[DisplayInfoSubFunction::PlaneDamage]) };
}

QList<QLabel*> HumaneLabsRaidStrategy::getLabels()
{
    return { &labMissionName, &labPlaneDamage };
}

void HumaneLabsRaidStrategy::updateInfo()
{
    auto data = vehicleDamageFetcher.fetchData();
    updatePlaneDamageLabel(data);
}

const QString HumaneLabsRaidStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室");
}
