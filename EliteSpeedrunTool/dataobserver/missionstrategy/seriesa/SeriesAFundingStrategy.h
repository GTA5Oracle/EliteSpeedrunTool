#ifndef SERIESAFUNDINGSTRATEGY_H
#define SERIESAFUNDINGSTRATEGY_H

#include "../BaseMissionStrategy.h"

class SeriesAFundingStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit SeriesAFundingStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // SERIESAFUNDINGSTRATEGY_H
