#include "HeadshotFetcher.h"
#include "GlobalData.h"

HeadshotFetcher::HeadshotFetcher(QObject* parent)
    : BaseDataFetcher(parent)
{
}

QString HeadshotFetcher::getDisplayName()
{
    return tr("总爆头");
}

QString HeadshotFetcher::id()
{
    return "Headshot";
}

QVariant HeadshotFetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<int>(MemoryUtil::localHeadshot));
}

void HeadshotFetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getHeadshotPattern());
}

DisplayInfoSubFunction HeadshotFetcher::getType()
{
    return DisplayInfoSubFunction::Headshot;
}

QString HeadshotFetcher::getHeadshotPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "☠️%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
