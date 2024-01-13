#include "Kill4PFetcher.h"

Kill4PFetcher::Kill4PFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

int Kill4PFetcher::fetchData()
{
    return getLocalData(MemoryUtil::localKillHost);
}
