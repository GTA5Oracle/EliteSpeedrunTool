#include "KillHostFetcher.h"

KillHostFetcher::KillHostFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

int KillHostFetcher::fetchData()
{
    return getLocalData(MemoryUtil::localKillHost);
}
