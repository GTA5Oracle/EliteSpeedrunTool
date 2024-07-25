#include "KillFetcher.h"

#include "GlobalData.h"

KillFetcher::KillFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString KillFetcher::getDisplayName()
{
    return tr("总击杀");
}

QString KillFetcher::id()
{
    return "Kill";
}

QVariant KillFetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<int>(MemoryUtil::localKill1)
        + getLocalData<int>(MemoryUtil::localKill2)
        + getLocalData<int>(MemoryUtil::localKill3)
        + getLocalData<int>(MemoryUtil::localKill4));
}

void KillFetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getKillPattern());
}

DisplayInfoSubFunction KillFetcher::getType()
{
    return DisplayInfoSubFunction::Kill;
}

QString KillFetcher::getKillPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "💀%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
