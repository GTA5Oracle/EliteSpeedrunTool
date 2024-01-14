#include "RashkovskyDamageFetcher.h"

RashkovskyDamageFetcher::RashkovskyDamageFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

float RashkovskyDamageFetcher::fetchData()
{
    return getLocalData(MemoryUtil::localRashkovskyDamage);
}
