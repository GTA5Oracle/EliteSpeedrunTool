#include "CasinoHeistAggressiveStrategy.h"
#include "GlobalData.h"

CasinoHeistAggressiveStrategy::CasinoHeistAggressiveStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labHeadshot.setFont(font);
}

QList<QLabel*> CasinoHeistAggressiveStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayHeadshot;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> CasinoHeistAggressiveStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return { qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]) };
}

QList<QLabel*> CasinoHeistAggressiveStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labHeadshot;
}

void CasinoHeistAggressiveStrategy::updateInfo()
{
    auto data = headshotFetcher.fetchData();
    labDisplayHeadshot.setText(headshotPattern.arg(QString::number(data)));
    labHeadshot.setText(headshotPattern.arg(QString::number(data)));
}

const QString CasinoHeistAggressiveStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：气势汹汹");
}
