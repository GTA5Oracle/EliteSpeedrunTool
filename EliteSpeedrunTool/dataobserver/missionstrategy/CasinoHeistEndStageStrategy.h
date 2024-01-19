#ifndef CASINOHEISTENDSTAGESTRATEGY_H
#define CASINOHEISTENDSTAGESTRATEGY_H

#include "CasinoHeistAggressiveStrategy.h"

class CasinoHeistEndStageStrategy : public CasinoHeistAggressiveStrategy {
    Q_OBJECT
public:
    explicit CasinoHeistEndStageStrategy(QObject* parent = nullptr);
    const QString getDisplayName() override;
};

#endif // CASINOHEISTENDSTAGESTRATEGY_H
