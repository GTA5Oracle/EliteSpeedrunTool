#include "DoomsdayAct2Strategy.h"

DoomsdayAct2Strategy::DoomsdayAct2Strategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labVehicleDamage.setFont(font);
}

QList<QLabel*> DoomsdayAct2Strategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayVehicleDamage;
}

QList<QLabel*> DoomsdayAct2Strategy::getLabels()
{
    return QList<QLabel*>() << &labVehicleDamage;
}

void DoomsdayAct2Strategy::updateInfo()
{
    auto data = vehicleDamageFetcher.fetchData();
    auto text = vehicleDamagePattern.arg(QString::number(data, 'f', 2));
    labDisplayVehicleDamage.setText(text);
    labVehicleDamage.setText(text);
}

const QString DoomsdayAct2Strategy::getDisplayName()
{
    return tr("波格丹危机");
}
