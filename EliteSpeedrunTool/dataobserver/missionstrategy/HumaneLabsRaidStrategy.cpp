#include "HumaneLabsRaidStrategy.h"
#include "GlobalData.h"

HumaneLabsRaidStrategy::HumaneLabsRaidStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labVehicleDamage.setFont(font);

    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    initGlobalDataConnects(&labDisplayVehicleDamage, displayInfoSubFunctions[DisplayInfoSubFunction::VehicleDamage]);
}

QList<QLabel*> HumaneLabsRaidStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayVehicleDamage;
}

QList<QLabel*> HumaneLabsRaidStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labVehicleDamage;
}

void HumaneLabsRaidStrategy::updateInfo()
{
    auto data = vehicleDamageFetcher.fetchData();
    auto text = vehicleDamagePattern.arg(QString::number(data, 'f', 2));
    labDisplayVehicleDamage.setText(text);
    labVehicleDamage.setText(text);
}

const QString HumaneLabsRaidStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室");
}