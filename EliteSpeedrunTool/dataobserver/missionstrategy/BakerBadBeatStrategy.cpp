#include "BakerBadBeatStrategy.h"
#include "GlobalData.h"

BakerBadBeatStrategy::BakerBadBeatStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labTotalKill.setFont(font);
}

QList<QLabel*> BakerBadBeatStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayTotalKill;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> BakerBadBeatStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayTotalKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill]) };
}

QList<QLabel*> BakerBadBeatStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labTotalKill;
}

void BakerBadBeatStrategy::updateInfo()
{
    auto data = killHostFetcher.fetchData();
    auto text = killPattern.arg(QString::number(data));
    labDisplayTotalKill.setText(text);
    labTotalKill.setText(text);
}

const QString BakerBadBeatStrategy::getDisplayName()
{
    return tr("出奇制胜");
}
