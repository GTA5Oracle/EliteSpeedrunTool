#pragma once

#include "../BaseMissionStrategy.h"

class SeriesATrashTruckStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit SeriesATrashTruckStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
