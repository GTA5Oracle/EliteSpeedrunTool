#pragma once

#include "../BaseMissionStrategy.h"

class Act1DeadCourierStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit Act1DeadCourierStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};
