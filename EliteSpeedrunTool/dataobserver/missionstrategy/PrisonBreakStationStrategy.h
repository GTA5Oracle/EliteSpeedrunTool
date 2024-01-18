#ifndef PRISONBREAKSTATIONSTRATEGY_H
#define PRISONBREAKSTATIONSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/HeadshotFetcher.h"
#include "dataobserver/datafetcher/KillHostClient1Fetcher.h"
#include "dataobserver/datafetcher/VehicleDamageFetcher.h"

class PrisonBreakStationStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PrisonBreakStationStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    VehicleDamageFetcher vehicleDamageFetcher = VehicleDamageFetcher();
    HeadshotFetcher headshotFetcher = HeadshotFetcher();
    KillHostClient1Fetcher killHostClient1Fetcher = KillHostClient1Fetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayVehicleDamage = QLabel();
    QLabel labVehicleDamage = QLabel();
    QLabel labDisplayHeadshot = QLabel();
    QLabel labHeadshot = QLabel();
    QLabel labDisplayKill = QLabel();
    QLabel labKill = QLabel();
};

#endif // PRISONBREAKSTATIONSTRATEGY_H
