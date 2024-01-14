#include "KillHostClient1Fetcher.h"

KillHostClient1Fetcher::KillHostClient1Fetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

int KillHostClient1Fetcher::fetchData()
{
    return getLocalData(MemoryUtil::localKillHost) + getLocalData(MemoryUtil::localKillClient1);
}
