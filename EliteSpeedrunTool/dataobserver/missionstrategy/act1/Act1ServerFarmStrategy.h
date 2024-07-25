#ifndef SERVERFARMSTRATEGY_H
#define SERVERFARMSTRATEGY_H

#include "../BaseMissionStrategy.h"

class Act1ServerFarmStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit Act1ServerFarmStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // SERVERFARMSTRATEGY_H
