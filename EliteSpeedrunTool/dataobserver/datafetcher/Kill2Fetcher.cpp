#include "Kill2Fetcher.h"
#include "GlobalData.h"

Kill2Fetcher::Kill2Fetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString Kill2Fetcher::getDisplayName()
{
    return tr("击杀（小队 2）");
}

QString Kill2Fetcher::id()
{
    return "Kill2";
}

QVariant Kill2Fetcher::fetchData()
{
    return QVariant::fromValue(
        getLocalData<int>(MemoryUtil::localKill1)
        + getLocalData<int>(MemoryUtil::localKill2));
}

void Kill2Fetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getKillPattern());
}

DisplayInfoSubFunction Kill2Fetcher::getType()
{
    return DisplayInfoSubFunction::Kill;
}

QString Kill2Fetcher::getKillPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "2💀%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
