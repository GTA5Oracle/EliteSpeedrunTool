#include "DoomsdayAct2Strategy.h"
#include "GlobalData.h"

DoomsdayAct2Strategy::DoomsdayAct2Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> DoomsdayAct2Strategy::getDisplayLabels()
{
    return { &labDisplayPlaneDamage };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> DoomsdayAct2Strategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayPlaneDamage, displayInfoSubFunctions[DisplayInfoSubFunction::PlaneDamage]) };
}

QList<QLabel*> DoomsdayAct2Strategy::getLabels()
{
    return { &labMissionName, &labPlaneDamage };
}

void DoomsdayAct2Strategy::updateInfo()
{
    auto data = vehicleDamageFetcher.fetchData();
    updatePlaneDamageLabel(data);
}

const QString DoomsdayAct2Strategy::getDisplayName()
{
    return tr("波格丹危机");
}
