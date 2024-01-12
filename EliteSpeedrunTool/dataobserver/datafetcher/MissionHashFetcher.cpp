#include "MissionHashFetcher.h"

MissionHashFetcher::MissionHashFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

unsigned long long MissionHashFetcher::fetchData()
{
    return getGlobalData(MemoryUtil::globalMissionHash);
}
