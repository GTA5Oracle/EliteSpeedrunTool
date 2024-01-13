#include "HeadshotFetcher.h"

template class BaseDataFetcher<int>;

HeadshotFetcher::HeadshotFetcher(QObject* parent)
    : BaseDataFetcher<int>(parent)
{
}

int HeadshotFetcher::fetchData()
{
    return getLocalData(MemoryUtil::localHeadshot);
}
