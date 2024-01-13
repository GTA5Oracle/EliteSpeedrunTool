#include "FleecaJobStrategy.h"

FleecaJobStrategy::FleecaJobStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labVehicleDamage.setFont(font);
}

QList<QLabel*> FleecaJobStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayVehicleDamage;
}

QList<QLabel*> FleecaJobStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labVehicleDamage;
}

void FleecaJobStrategy::updateInfo()
{
    auto data = vehicleDamageFetcher.fetchData();
    auto text = vehicleDamagePattern.arg(QString::number(data, 'f', 2));
    labDisplayVehicleDamage.setText(text);
    labVehicleDamage.setText(text);
}

const QString FleecaJobStrategy::getDisplayName()
{
    return tr("全福银行差事");
}
