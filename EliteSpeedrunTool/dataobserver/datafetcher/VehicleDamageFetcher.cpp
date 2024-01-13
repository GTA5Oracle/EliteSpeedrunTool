#include "VehicleDamageFetcher.h"

VehicleDamageFetcher::VehicleDamageFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

float VehicleDamageFetcher::fetchData()
{
    return getLocalData(MemoryUtil::localVehicleDamage);
}
