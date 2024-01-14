#include "DoomsdayAct1Strategy.h"
#include "GlobalData.h"

DoomsdayAct1Strategy::DoomsdayAct1Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labTotalKill.setFont(font);

    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    initGlobalDataConnects(&labDisplayTotalKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill]);
}

QList<QLabel*> DoomsdayAct1Strategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayTotalKill;
}

QList<QLabel*> DoomsdayAct1Strategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labTotalKill;
}

void DoomsdayAct1Strategy::updateInfo()
{
    auto data = killHostFetcher.fetchData();
    auto text = killPattern.arg(QString::number(data));
    labDisplayTotalKill.setText(text);
    labTotalKill.setText(text);
}

const QString DoomsdayAct1Strategy::getDisplayName()
{
    return tr("数据泄露");
}
