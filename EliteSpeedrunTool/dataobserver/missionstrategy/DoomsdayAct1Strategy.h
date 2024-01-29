#ifndef DOOMSDAYACT1STRATEGY_H
#define DOOMSDAYACT1STRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/KillHostFetcher.h"

class DoomsdayAct1Strategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit DoomsdayAct1Strategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    KillHostFetcher killHostFetcher = KillHostFetcher();
};

#endif // DOOMSDAYACT1STRATEGY_H
