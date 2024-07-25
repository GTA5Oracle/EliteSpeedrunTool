#include "RashkovskyDamageFetcher.h"
#include "GlobalData.h"

RashkovskyDamageFetcher::RashkovskyDamageFetcher(QObject* parent)
    : BaseDataFetcher { parent }
{
}

QString RashkovskyDamageFetcher::getDisplayName()
{
    return tr("拉什科夫斯基受伤");
}

QString RashkovskyDamageFetcher::id()
{
    return "RashkovskyHurt";
}

QVariant RashkovskyDamageFetcher::fetchData()
{
    return QVariant::fromValue(getLocalData<float>(MemoryUtil::localRashkovskyDamage));
}

void RashkovskyDamageFetcher::fetchDataAndUpdateLabel()
{
    updateLabel(fetchData(), getRashkovskyHurtPattern());
}

DisplayInfoSubFunction RashkovskyDamageFetcher::getType()
{
    return DisplayInfoSubFunction::RashkovskyDamage;
}

QString RashkovskyDamageFetcher::getRashkovskyHurtPattern()
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
