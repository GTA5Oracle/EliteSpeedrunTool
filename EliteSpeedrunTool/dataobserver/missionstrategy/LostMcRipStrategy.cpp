#include "LostMcRipStrategy.h"
#include "GlobalData.h"

LostMcRipStrategy::LostMcRipStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labHeadshot.setFont(font);
    labKill.setFont(font);
}

QList<QLabel*> LostMcRipStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayHeadshot << &labDisplayKill;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> LostMcRipStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return {
        qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]),
        qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill])
    };
}

QList<QLabel*> LostMcRipStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labHeadshot << &labKill;
}

void LostMcRipStrategy::updateInfo()
{
    auto headshot = headshotFetcher.fetchData();
    auto headshotText = getHeadshotPattern().arg(QString::number(headshot));
    labDisplayHeadshot.setText(headshotText);
    labHeadshot.setText(headshotText);

    auto killHost = killHostFetcher.fetchData();
    auto killText = getKillPattern().arg(QString::number(killHost));
    labDisplayKill.setText(killText);
    labKill.setText(killText);
}

const QString LostMcRipStrategy::getDisplayName()
{
    return tr("安息吧！失落摩托帮");
}
