#include "PrisonBreakWetWorkStrategy.h"
#include "GlobalData.h"

PrisonBreakWetWorkStrategy::PrisonBreakWetWorkStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> PrisonBreakWetWorkStrategy::getDisplayLabels()
{
    return { &labDisplayHeadshot, &labDisplayKill };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> PrisonBreakWetWorkStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return {
        qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]),
        qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill])
    };
}

QList<QLabel*> PrisonBreakWetWorkStrategy::getLabels()
{
    return { &labMissionName, &labHeadshot, &labKill };
}

void PrisonBreakWetWorkStrategy::updateInfo()
{
    auto headshot = headshotFetcher.fetchData();
    updateHeadshotLabel(headshot);

    auto killHostClient1 = killHostClient1Fetcher.fetchData();
    updateKillLabel(killHostClient1);
}

const QString PrisonBreakWetWorkStrategy::getDisplayName()
{
    return tr("越狱：行刺");
}
