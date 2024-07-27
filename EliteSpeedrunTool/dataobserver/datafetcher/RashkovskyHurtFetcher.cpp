#include "RashkovskyHurtFetcher.h"
#include "GlobalData.h"

RashkovskyHurtFetcher::RashkovskyHurtFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString RashkovskyHurtFetcher::getDisplayName()
{
    return tr("拉什科夫斯基受伤");
}

QString RashkovskyHurtFetcher::id()
{
    return "RashkovskyHurt";
}

QVariant RashkovskyHurtFetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<float>(MemoryUtil::localRashkovskyDamage));
}

void RashkovskyHurtFetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getRashkovskyHurtPattern());
}

DisplayInfoSubFunction RashkovskyHurtFetcher::getType()
{
    return DisplayInfoSubFunction::RashkovskyDamage;
}

QString RashkovskyHurtFetcher::getRashkovskyHurtPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "👨🏻‍🦲%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return getDisplayName() + ": " + "%1";
    }
}
