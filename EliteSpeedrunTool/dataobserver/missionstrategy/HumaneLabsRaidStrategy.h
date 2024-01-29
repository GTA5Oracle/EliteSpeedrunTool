#ifndef HUMANELABSRAIDSTRATEGY_H
#define HUMANELABSRAIDSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/VehicleDamageFetcher.h"

class HumaneLabsRaidStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit HumaneLabsRaidStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    VehicleDamageFetcher vehicleDamageFetcher = VehicleDamageFetcher();
};

#endif // HUMANELABSRAIDSTRATEGY_H
