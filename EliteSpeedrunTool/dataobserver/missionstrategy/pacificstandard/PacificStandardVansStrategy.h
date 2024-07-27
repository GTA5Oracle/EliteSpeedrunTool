#pragma once

#include "../BaseMissionStrategy.h"

class PacificStandardVansStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PacificStandardVansStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
