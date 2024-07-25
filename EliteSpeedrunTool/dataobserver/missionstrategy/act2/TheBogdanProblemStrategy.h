#ifndef DOOMSDAYACT2STRATEGY_H
#define DOOMSDAYACT2STRATEGY_H

#include "../BaseMissionStrategy.h"

class TheBogdanProblemStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit TheBogdanProblemStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // DOOMSDAYACT2STRATEGY_H
