#include "HeadshotFetcher.h"
#include "TimeUtil.h"

template class BaseDataFetcher<int>;

HeadshotFetcher::HeadshotFetcher(QObject* parent)
    : BaseDataFetcher<int>(parent)
{
}

int HeadshotFetcher::fetchData()
{
    return TimeUtil::currentTimestamp(); // getLocalData(MemoryUtil::localHeadshot);
}
