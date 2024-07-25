#ifndef PRISONBREAKWETWORKSTRATEGY_H
#define PRISONBREAKWETWORKSTRATEGY_H

#include "../BaseMissionStrategy.h"

class PrisonBreakWetWorkStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PrisonBreakWetWorkStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // PRISONBREAKWETWORKSTRATEGY_H
