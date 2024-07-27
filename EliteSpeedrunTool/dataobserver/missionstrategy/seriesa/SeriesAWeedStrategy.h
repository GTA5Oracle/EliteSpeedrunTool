#pragma once

#include "../BaseMissionStrategy.h"

class SeriesAWeedStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit SeriesAWeedStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
