#ifndef CASINOHEISTSILENTSNEAKYSTRATEGY_H
#define CASINOHEISTSILENTSNEAKYSTRATEGY_H

#include "../BaseMissionStrategy.h"

class CasinoHeistSilentSneakyStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit CasinoHeistSilentSneakyStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // CASINOHEISTSILENTSNEAKYSTRATEGY_H