#include "PrisonBreakWetWorkStrategy.h"
#include "GlobalData.h"

PrisonBreakWetWorkStrategy::PrisonBreakWetWorkStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labHeadshot.setFont(font);
    labKill.setFont(font);
}

QList<QLabel*> PrisonBreakWetWorkStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayHeadshot << &labDisplayKill;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> PrisonBreakWetWorkStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return {
        qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]),
        qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill])
    };
}

QList<QLabel*> PrisonBreakWetWorkStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labHeadshot << &labKill;
}

void PrisonBreakWetWorkStrategy::updateInfo()
{
    auto headshot = headshotFetcher.fetchData();
    auto headshotText = getHeadshotPattern().arg(QString::number(headshot));
    labDisplayHeadshot.setText(headshotText);
    labHeadshot.setText(headshotText);

    auto killHostClient1 = killHostClient1Fetcher.fetchData();
    auto killText = getKillPattern().arg(QString::number(killHostClient1));
    labDisplayKill.setText(killText);
    labKill.setText(killText);
}

const QString PrisonBreakWetWorkStrategy::getDisplayName()
{
    return tr("越狱：行刺");
}
