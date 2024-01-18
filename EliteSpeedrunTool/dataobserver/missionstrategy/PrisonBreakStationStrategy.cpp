#include "PrisonBreakStationStrategy.h"
#include "GlobalData.h"

PrisonBreakStationStrategy::PrisonBreakStationStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labVehicleDamage.setFont(font);
    labHeadshot.setFont(font);
    labKill.setFont(font);
}

QList<QLabel*> PrisonBreakStationStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayVehicleDamage << &labDisplayHeadshot << &labDisplayKill;
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> PrisonBreakStationStrategy::getDisplayLabelsAndItems()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    return {
        qMakePair(&labDisplayVehicleDamage, displayInfoSubFunctions[DisplayInfoSubFunction::VehicleDamage]),
        qMakePair(&labDisplayHeadshot, displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]),
        qMakePair(&labDisplayKill, displayInfoSubFunctions[DisplayInfoSubFunction::Kill])
    };
}

QList<QLabel*> PrisonBreakStationStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labVehicleDamage << &labHeadshot << &labKill;
}

void PrisonBreakStationStrategy::updateInfo()
{
    auto vehicleDamage = vehicleDamageFetcher.fetchData();
    auto vehicleDamagetext = getVehicleDamagePattern().arg(QString::number(vehicleDamage, 'f', 2));
    labDisplayVehicleDamage.setText(vehicleDamagetext);
    labVehicleDamage.setText(vehicleDamagetext);

    auto headshot = headshotFetcher.fetchData();
    auto headshotText = getHeadshotPattern().arg(QString::number(headshot));
    labDisplayHeadshot.setText(headshotText);
    labHeadshot.setText(headshotText);

    auto killHostClient1 = killHostClient1Fetcher.fetchData();
    auto killText = getKillPattern().arg(QString::number(killHostClient1));
    labDisplayKill.setText(killText);
    labKill.setText(killText);
}

const QString PrisonBreakStationStrategy::getDisplayName()
{
    return tr("越狱：警察局");
}
