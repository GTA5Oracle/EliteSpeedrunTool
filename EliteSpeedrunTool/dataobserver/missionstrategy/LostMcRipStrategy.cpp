#include "LostMcRipStrategy.h"
#include "GlobalData.h"

LostMcRipStrategy::LostMcRipStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> LostMcRipStrategy::getDisplayLabels()
{
    return { &labDisplayHeadshot, &labDisplayKill };
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
    return { &labMissionName, &labHeadshot, &labKill };
}

void LostMcRipStrategy::updateInfo()
{
    auto headshot = headshotFetcher.fetchData();
    updateHeadshotLabel(headshot);

    auto killHost = killHostFetcher.fetchData();
    updateKillLabel(killHost);
}

const QString LostMcRipStrategy::getDisplayName()
{
    return tr("安息吧！失落摩托帮");
}
