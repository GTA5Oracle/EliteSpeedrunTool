#pragma once

#include "../BaseMissionStrategy.h"

class FleecaJobScopeOutStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit FleecaJobScopeOutStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
