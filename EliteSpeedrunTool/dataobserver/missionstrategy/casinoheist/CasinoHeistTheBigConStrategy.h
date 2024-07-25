#ifndef CASINOHEISTTHEBIGCONSTRATEGY_H
#define CASINOHEISTTHEBIGCONSTRATEGY_H

#include "../BaseMissionStrategy.h"

class CasinoHeistTheBigConStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit CasinoHeistTheBigConStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // CASINOHEISTTHEBIGCONSTRATEGY_H
