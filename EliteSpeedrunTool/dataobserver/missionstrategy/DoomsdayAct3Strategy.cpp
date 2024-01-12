#include "DoomsdayAct3Strategy.h"
Q_GLOBAL_STATIC(DoomsdayAct3Strategy, doomsdayAct3StrategyInstance)

DoomsdayAct3Strategy::DoomsdayAct3Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

DoomsdayAct3Strategy::~DoomsdayAct3Strategy()
{
    delete labDisplayHeadshot;
    delete labHeadshot;
}

DoomsdayAct3Strategy* DoomsdayAct3Strategy::instance()
{
    return doomsdayAct3StrategyInstance;
}

QList<QLabel*> DoomsdayAct3Strategy::getDisplayLabels()
{
    return QList<QLabel*>() << labDisplayHeadshot;
}

QList<QLabel*> DoomsdayAct3Strategy::getLabels()
{
    return QList<QLabel*>() << labHeadshot;
}

void DoomsdayAct3Strategy::updateInfo()
{
    auto data = headshotFetcher.fetchData();
    labDisplayHeadshot->setText(QString::number(data));
    labHeadshot->setText(QString::number(data));
}
