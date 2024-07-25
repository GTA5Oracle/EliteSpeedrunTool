#pragma once

#include "../BaseMissionStrategy.h"

class FleecaJobKurumaStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit FleecaJobKurumaStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
