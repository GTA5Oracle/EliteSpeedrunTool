#include "SeriesAFundingStrategy.h"
#include "GlobalData.h"

SeriesAFundingStrategy::SeriesAFundingStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> SeriesAFundingStrategy::getDisplayLabels()
{
    return { &labDisplayKill };
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> SeriesAFundingStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill]) };
}

QList<QLabel*> SeriesAFundingStrategy::getLabels()
{
    return { &labMissionName, &labKill };
}

void SeriesAFundingStrategy::updateInfo()
{
    auto data = killHostClient1Fetcher.fetchData();
    updateKillLabel(data);
}

const QString SeriesAFundingStrategy::getDisplayName()
{
    return tr("首轮募资");
}
