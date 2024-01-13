#include "LostMcRipStrategy.h"

LostMcRipStrategy::LostMcRipStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labHeadshot.setFont(font);
    labKill4P.setFont(font);
}

QList<QLabel*> LostMcRipStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayHeadshot << &labDisplayKill4P;
}

QList<QLabel*> LostMcRipStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labHeadshot << &labKill4P;
}

void LostMcRipStrategy::updateInfo()
{
    auto headshot = headshotFetcher.fetchData();
    qDebug() << headshot;
    labDisplayHeadshot.setText(headshotPattern.arg(QString::number(headshot)));
    labHeadshot.setText(headshotPattern.arg(QString::number(headshot)));

    auto killHost = kill4PFetcher.fetchData();
    qDebug() << killHost;
    labDisplayKill4P.setText(kill4PPattern.arg(QString::number(killHost)));
    labKill4P.setText(kill4PPattern.arg(QString::number(killHost)));
}

const QString LostMcRipStrategy::getDisplayName()
{
    return tr("安息吧！失落摩托帮");
}
