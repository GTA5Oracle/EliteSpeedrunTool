#ifndef CASINOHEISTAGGRESSIVESTRATEGY_H
#define CASINOHEISTAGGRESSIVESTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/HeadshotFetcher.h"

class CasinoHeistAggressiveStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit CasinoHeistAggressiveStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    HeadshotFetcher headshotFetcher = HeadshotFetcher();
};

#endif // CASINOHEISTAGGRESSIVESTRATEGY_H
