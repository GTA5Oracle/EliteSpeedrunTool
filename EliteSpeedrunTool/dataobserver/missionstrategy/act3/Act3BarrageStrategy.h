#pragma once

#include "../BaseMissionStrategy.h"

class Act3BarrageStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit Act3BarrageStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
