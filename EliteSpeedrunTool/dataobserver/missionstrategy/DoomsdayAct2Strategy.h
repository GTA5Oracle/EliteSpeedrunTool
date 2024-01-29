#ifndef DOOMSDAYACT2STRATEGY_H
#define DOOMSDAYACT2STRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/VehicleDamageFetcher.h"

class DoomsdayAct2Strategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit DoomsdayAct2Strategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    VehicleDamageFetcher vehicleDamageFetcher = VehicleDamageFetcher();
};

#endif // DOOMSDAYACT2STRATEGY_H
