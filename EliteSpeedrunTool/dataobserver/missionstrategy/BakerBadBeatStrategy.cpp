#include "BakerBadBeatStrategy.h"
#include "GlobalData.h"

BakerBadBeatStrategy::BakerBadBeatStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> BakerBadBeatStrategy::getDisplayLabels()
{
    return { &labDisplayKill };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> BakerBadBeatStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill]) };
}

QList<QLabel*> BakerBadBeatStrategy::getLabels()
{
    return { &labMissionName, &labKill };
}

void BakerBadBeatStrategy::updateInfo()
{
    auto data = killHostFetcher.fetchData();
    updateKillLabel(data);
}

const QString BakerBadBeatStrategy::getDisplayName()
{
    return tr("出奇制胜");
}
