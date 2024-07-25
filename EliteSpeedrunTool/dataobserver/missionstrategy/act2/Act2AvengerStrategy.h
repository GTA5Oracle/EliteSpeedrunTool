#pragma once

#include "../BaseMissionStrategy.h"

class Act2AvengerStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit Act2AvengerStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
