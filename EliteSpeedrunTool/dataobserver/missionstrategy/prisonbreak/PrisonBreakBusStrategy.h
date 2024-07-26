#pragma once

#include "../BaseMissionStrategy.h"

class PrisonBreakBusStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PrisonBreakBusStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
