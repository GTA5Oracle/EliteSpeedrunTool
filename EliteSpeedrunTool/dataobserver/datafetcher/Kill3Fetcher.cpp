#include "Kill3Fetcher.h"

#include "GlobalData.h"

Kill3Fetcher::Kill3Fetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString Kill3Fetcher::getDisplayName()
{
    return tr("击杀（小队 3）");
}

QString Kill3Fetcher::id()
{
    return "Kill3";
}

QVariant Kill3Fetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<int>(MemoryUtil::localKill3));
}

void Kill3Fetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getKillPattern());
}

DisplayInfoSubFunction Kill3Fetcher::getType()
{
    return DisplayInfoSubFunction::Kill;
}

QString Kill3Fetcher::getKillPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "3💀%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
