#ifndef SERIESAFUNDINGSTRATEGY_H
#define SERIESAFUNDINGSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/KillHostClient1Fetcher.h"

class SeriesAFundingStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit SeriesAFundingStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    KillHostClient1Fetcher killHostClient1Fetcher = KillHostClient1Fetcher();
};

#endif // SERIESAFUNDINGSTRATEGY_H
