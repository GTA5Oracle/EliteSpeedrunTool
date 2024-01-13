#include "MissionHashFetcher.h"

MissionHashFetcher::MissionHashFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

unsigned int MissionHashFetcher::fetchData()
{
    return getGlobalData(MemoryUtil::globalMissionHash);
}
