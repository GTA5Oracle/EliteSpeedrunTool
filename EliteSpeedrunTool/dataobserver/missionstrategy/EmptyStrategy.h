#ifndef EMPTYSTRATEGY_H
#define EMPTYSTRATEGY_H

#include "BaseMissionStrategy.h"

class EmptyStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit EmptyStrategy(QObject* parent = nullptr);

    const QString getDisplayName() override;

    QSet<QString> defaultDataFetchers() override;

    QString id() override;

    const QSet<unsigned long long> missionHash() override;
};

#endif // EMPTYSTRATEGY_H
