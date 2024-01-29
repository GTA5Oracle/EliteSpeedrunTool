#include "PrisonBreakStrategy.h"
#include "GlobalData.h"

PrisonBreakStrategy::PrisonBreakStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> PrisonBreakStrategy::getDisplayLabels()
{
    return { &labDisplayRashkovskyDamage };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> PrisonBreakStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayRashkovskyDamage, displayInfoSubFunctions[DisplayInfoSubFunction::RashkovskyDamage]) };
}

QList<QLabel*> PrisonBreakStrategy::getLabels()
{
    return { &labMissionName, &labRashkovskyDamage };
}

void PrisonBreakStrategy::updateInfo()
{
    auto data = rashkovskyDamageFetcher.fetchData();
    updateRashkovskyHurtLabel(data);
}

const QString PrisonBreakStrategy::getDisplayName()
{
    return tr("越狱");
}
