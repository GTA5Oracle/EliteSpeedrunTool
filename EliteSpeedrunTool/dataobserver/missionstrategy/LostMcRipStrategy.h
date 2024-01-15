#ifndef LOSTMCRIPSTRATEGY_H
#define LOSTMCRIPSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/HeadshotFetcher.h"
#include "dataobserver/datafetcher/KillHostFetcher.h"

class LostMcRipStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit LostMcRipStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    HeadshotFetcher headshotFetcher = HeadshotFetcher();
    KillHostFetcher killHostFetcher = KillHostFetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayHeadshot = QLabel();
    QLabel labHeadshot = QLabel();
    QLabel labDisplayKill = QLabel();
    QLabel labKill = QLabel();
};

#endif // LOSTMCRIPSTRATEGY_H
