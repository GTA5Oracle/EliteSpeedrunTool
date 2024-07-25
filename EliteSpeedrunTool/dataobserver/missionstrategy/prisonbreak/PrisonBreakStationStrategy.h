#ifndef PRISONBREAKSTATIONSTRATEGY_H
#define PRISONBREAKSTATIONSTRATEGY_H

#include "../BaseMissionStrategy.h"

class PrisonBreakStationStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PrisonBreakStationStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // PRISONBREAKSTATIONSTRATEGY_H
