#include "FleecaJobStrategy.h"
#include "GlobalData.h"

FleecaJobStrategy::FleecaJobStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> FleecaJobStrategy::getDisplayLabels()
{
    return { &labDisplayVehicleDamage };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> FleecaJobStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayVehicleDamage, displayInfoSubFunctions[DisplayInfoSubFunction::VehicleDamage]) };
}

QList<QLabel*> FleecaJobStrategy::getLabels()
{
    return { &labMissionName, &labVehicleDamage };
}

void FleecaJobStrategy::updateInfo()
{
    auto data = vehicleDamageFetcher.fetchData();
    updateVehicleDamageLabel(data);
}

const QString FleecaJobStrategy::getDisplayName()
{
    return tr("全福银行差事");
}
