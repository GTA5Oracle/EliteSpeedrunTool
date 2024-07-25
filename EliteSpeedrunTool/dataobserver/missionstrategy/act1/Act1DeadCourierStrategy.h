#ifndef DEADCOURIERSTRATEGY_H
#define DEADCOURIERSTRATEGY_H

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

#endif // DEADCOURIERSTRATEGY_H
