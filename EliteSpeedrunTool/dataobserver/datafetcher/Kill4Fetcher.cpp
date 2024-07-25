#include "Kill4Fetcher.h"

#include "GlobalData.h"

Kill4Fetcher::Kill4Fetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString Kill4Fetcher::getDisplayName()
{
    return tr("击杀（小队 4）");
}

QString Kill4Fetcher::id()
{
    return "Kill4";
}

QVariant Kill4Fetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<int>(MemoryUtil::localKill4));
}

void Kill4Fetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getKillPattern());
}

DisplayInfoSubFunction Kill4Fetcher::getType()
{
    return DisplayInfoSubFunction::Kill;
}

QString Kill4Fetcher::getKillPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "4💀%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
