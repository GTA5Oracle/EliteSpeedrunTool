#ifndef FLEECAJOBSTRATEGY_H
#define FLEECAJOBSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/VehicleDamageFetcher.h"

class FleecaJobStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit FleecaJobStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    VehicleDamageFetcher vehicleDamageFetcher = VehicleDamageFetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayVehicleDamage = QLabel();
    QLabel labVehicleDamage = QLabel();
};

#endif // FLEECAJOBSTRATEGY_H
