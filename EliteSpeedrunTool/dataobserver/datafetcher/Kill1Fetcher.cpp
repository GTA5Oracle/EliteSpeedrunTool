#include "Kill1Fetcher.h"

#include "GlobalData.h"

Kill1Fetcher::Kill1Fetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString Kill1Fetcher::getDisplayName()
{
    return tr("击杀（小队 1）");
}

QString Kill1Fetcher::id()
{
    return "Kill1";
}

QVariant Kill1Fetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<int>(MemoryUtil::localKill1));
}

void Kill1Fetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getKillPattern());
}

DisplayInfoSubFunction Kill1Fetcher::getType()
{
    return DisplayInfoSubFunction::Kill;
}

QString Kill1Fetcher::getKillPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "1💀%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
