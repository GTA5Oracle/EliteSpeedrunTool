#include "FleecaJobStrategy.h"
#include "GlobalData.h"

FleecaJobStrategy::FleecaJobStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labVehicleDamage.setFont(font);
}

QList<QLabel*> FleecaJobStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayVehicleDamage;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> FleecaJobStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayVehicleDamage, displayInfoSubFunctions[DisplayInfoSubFunction::VehicleDamage]) };
}

QList<QLabel*> FleecaJobStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labVehicleDamage;
}

void FleecaJobStrategy::updateInfo()
{
    auto data = vehicleDamageFetcher.fetchData();
    auto text = getVehicleDamagePattern().arg(QString::number(data, 'f', 2));
    labDisplayVehicleDamage.setText(text);
    labVehicleDamage.setText(text);
}

const QString FleecaJobStrategy::getDisplayName()
{
    return tr("全福银行差事");
}
