#include "VehicleDamageFetcher.h"
#include "GlobalData.h"

VehicleDamageFetcher::VehicleDamageFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString VehicleDamageFetcher::getDisplayName()
{
    return tr("载具受损");
}

QString VehicleDamageFetcher::id()
{
    return "VehicleDamage";
}

QVariant VehicleDamageFetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<float>(MemoryUtil::localVehicleDamage));
}

void VehicleDamageFetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getVehicleDamagePattern());
}

DisplayInfoSubFunction VehicleDamageFetcher::getType()
{
    return DisplayInfoSubFunction::VehicleDamage;
}

QString VehicleDamageFetcher::getVehicleDamagePattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "🚗%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
