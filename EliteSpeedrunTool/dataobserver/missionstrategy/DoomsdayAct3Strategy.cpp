#include "DoomsdayAct3Strategy.h"
Q_GLOBAL_STATIC(DoomsdayAct3Strategy, doomsdayAct3StrategyInstance)

DoomsdayAct3Strategy::DoomsdayAct3Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labHeadshot.setFont(font);
}

DoomsdayAct3Strategy* DoomsdayAct3Strategy::instance()
{
    return doomsdayAct3StrategyInstance;
}

QList<QLabel*> DoomsdayAct3Strategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayHeadshot;
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
