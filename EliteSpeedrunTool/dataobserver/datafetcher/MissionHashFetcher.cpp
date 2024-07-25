#include "MissionHashFetcher.h"

MissionHashFetcher::MissionHashFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString MissionHashFetcher::getDisplayName()
{
    return tr("任务 ID");
}

QString MissionHashFetcher::id()
{
    return "MissionHash";
}

void MissionHashFetcher::fetchDataAndUpdateLabel()
{
}

QVariant MissionHashFetcher::fetchData()
{
    return QVariant::fromValue(getGlobalData<unsigned int>(MemoryUtil::globalMissionHash));
}

DisplayInfoSubFunction MissionHashFetcher::getType()
{
    return DisplayInfoSubFunction::Firewall; // Meaningless
}
