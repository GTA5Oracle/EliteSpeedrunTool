#ifndef BAKERBADBEATSTRATEGY_H
#define BAKERBADBEATSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/KillHostFetcher.h"

class BakerBadBeatStrategy : public BaseMissionStrategy {
public:
    explicit BakerBadBeatStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    KillHostFetcher killHostFetcher = KillHostFetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayTotalKill = QLabel();
    QLabel labTotalKill = QLabel();
};

#endif // BAKERBADBEATSTRATEGY_H
