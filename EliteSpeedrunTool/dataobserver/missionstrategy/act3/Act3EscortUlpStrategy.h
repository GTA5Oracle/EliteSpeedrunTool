#pragma once

#include "../BaseMissionStrategy.h"

class Act3EscortUlpStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit Act3EscortUlpStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
