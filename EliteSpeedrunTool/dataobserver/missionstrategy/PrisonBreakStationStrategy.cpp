#include "PrisonBreakStationStrategy.h"
#include "GlobalData.h"

PrisonBreakStationStrategy::PrisonBreakStationStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> PrisonBreakStationStrategy::getDisplayLabels()
{
    return { &labDisplayVehicleDamage, &labDisplayHeadshot, &labDisplayKill };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> PrisonBreakStationStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return {
        qMakePair(&labDisplayVehicleDamage, displayInfoSubFunctions[DisplayInfoSubFunction::VehicleDamage]),
        qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]),
        qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill])
    };
}

QList<QLabel*> PrisonBreakStationStrategy::getLabels()
{
    return { &labMissionName, &labVehicleDamage, &labHeadshot, &labKill };
}

void PrisonBreakStationStrategy::updateInfo()
{
    auto vehicleDamage = vehicleDamageFetcher.fetchData();
    updateVehicleDamageLabel(vehicleDamage);

    auto headshot = headshotFetcher.fetchData();
    updateHeadshotLabel(headshot);

    auto killHostClient1 = killHostClient1Fetcher.fetchData();
    updateKillLabel(killHostClient1);
}

const QString PrisonBreakStationStrategy::getDisplayName()
{
    return tr("越狱：警察局");
}
