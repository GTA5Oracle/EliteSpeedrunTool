#ifndef PRISONBREAKWETWORKSTRATEGY_H
#define PRISONBREAKWETWORKSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/HeadshotFetcher.h"
#include "dataobserver/datafetcher/KillHostClient1Fetcher.h"

class PrisonBreakWetWorkStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PrisonBreakWetWorkStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    HeadshotFetcher headshotFetcher = HeadshotFetcher();
    KillHostClient1Fetcher killHostClient1Fetcher = KillHostClient1Fetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayHeadshot = QLabel();
    QLabel labHeadshot = QLabel();
    QLabel labDisplayKill = QLabel();
    QLabel labKill = QLabel();
};

#endif // PRISONBREAKWETWORKSTRATEGY_H
