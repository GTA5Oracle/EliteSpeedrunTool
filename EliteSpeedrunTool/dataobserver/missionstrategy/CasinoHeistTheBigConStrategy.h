#ifndef CASINOHEISTTHEBIGCONSTRATEGY_H
#define CASINOHEISTTHEBIGCONSTRATEGY_H

#include "BaseMissionStrategy.h"

class CasinoHeistTheBigConStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit CasinoHeistTheBigConStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;
};

#endif // CASINOHEISTTHEBIGCONSTRATEGY_H
