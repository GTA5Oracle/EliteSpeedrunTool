#ifndef SIGNALINTERCEPTSSTRATEGY_H
#define SIGNALINTERCEPTSSTRATEGY_H

#include "../BaseMissionStrategy.h"

class Act1SignalInterceptsStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit Act1SignalInterceptsStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // SIGNALINTERCEPTSSTRATEGY_H
