#ifndef LOSTMCRIPSTRATEGY_H
#define LOSTMCRIPSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/HeadshotFetcher.h"
#include "dataobserver/datafetcher/Kill4PFetcher.h"

class LostMcRipStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit LostMcRipStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    HeadshotFetcher headshotFetcher = HeadshotFetcher();
    Kill4PFetcher kill4PFetcher = Kill4PFetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayHeadshot = QLabel();
    QLabel labHeadshot = QLabel();
    QLabel labDisplayKill4P = QLabel();
    QLabel labKill4P = QLabel();
};

#endif // LOSTMCRIPSTRATEGY_H
