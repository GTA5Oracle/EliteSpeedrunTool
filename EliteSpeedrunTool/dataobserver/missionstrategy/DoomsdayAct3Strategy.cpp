#include "DoomsdayAct3Strategy.h"
#include "GlobalData.h"

DoomsdayAct3Strategy::DoomsdayAct3Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labHeadshot.setFont(font);
}

QList<QLabel*> DoomsdayAct3Strategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayHeadshot;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> DoomsdayAct3Strategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]) };
}

QList<QLabel*> DoomsdayAct3Strategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labHeadshot;
}

void DoomsdayAct3Strategy::updateInfo()
{
    auto data = headshotFetcher.fetchData();
    labDisplayHeadshot.setText(headshotPattern.arg(QString::number(data)));
    labHeadshot.setText(headshotPattern.arg(QString::number(data)));
}

const QString DoomsdayAct3Strategy::getDisplayName()
{
    return tr("末日将至");
}
