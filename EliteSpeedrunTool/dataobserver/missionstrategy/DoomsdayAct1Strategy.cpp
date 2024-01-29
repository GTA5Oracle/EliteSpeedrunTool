#include "DoomsdayAct1Strategy.h"
#include "GlobalData.h"

DoomsdayAct1Strategy::DoomsdayAct1Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> DoomsdayAct1Strategy::getDisplayLabels()
{
    return { &labDisplayKill };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> DoomsdayAct1Strategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill]) };
}

QList<QLabel*> DoomsdayAct1Strategy::getLabels()
{
    return { &labMissionName, &labKill };
}

void DoomsdayAct1Strategy::updateInfo()
{
    auto data = killHostFetcher.fetchData();
    updateKillLabel(data);
}

const QString DoomsdayAct1Strategy::getDisplayName()
{
    return tr("数据泄露");
}
