#include "SeriesAFundingStrategy.h"
#include "GlobalData.h"

SeriesAFundingStrategy::SeriesAFundingStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labTotalKill.setFont(font);
}

QList<QLabel*> SeriesAFundingStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayTotalKill;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> SeriesAFundingStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayTotalKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill]) };
}

QList<QLabel*> SeriesAFundingStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labTotalKill;
}

void SeriesAFundingStrategy::updateInfo()
{
    auto data = killHostClient1Fetcher.fetchData();
    auto text = killPattern.arg(QString::number(data));
    labDisplayTotalKill.setText(text);
    labTotalKill.setText(text);
}

const QString SeriesAFundingStrategy::getDisplayName()
{
    return tr("首轮募资");
}
