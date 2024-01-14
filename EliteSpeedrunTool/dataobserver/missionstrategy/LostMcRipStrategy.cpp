#include "LostMcRipStrategy.h"
#include "GlobalData.h"

LostMcRipStrategy::LostMcRipStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labHeadshot.setFont(font);
    labKill.setFont(font);

    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    initGlobalDataConnects(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]);
    initGlobalDataConnects(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill]);
}

QList<QLabel*> LostMcRipStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayHeadshot << &labDisplayKill;
}

QList<QLabel*> LostMcRipStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labHeadshot << &labKill;
}

void LostMcRipStrategy::updateInfo()
{
    auto headshot = headshotFetcher.fetchData();
    labDisplayHeadshot.setText(headshotPattern.arg(QString::number(headshot)));
    labHeadshot.setText(headshotPattern.arg(QString::number(headshot)));

    auto killHost = killHostFetcher.fetchData();
    labDisplayKill.setText(killPattern.arg(QString::number(killHost)));
    labKill.setText(killPattern.arg(QString::number(killHost)));
}

const QString LostMcRipStrategy::getDisplayName()
{
    return tr("安息吧！失落摩托帮");
}
