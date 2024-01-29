#ifndef CASINOHEISTSILENTSNEAKYSTRATEGY_H
#define CASINOHEISTSILENTSNEAKYSTRATEGY_H

#include "BaseMissionStrategy.h"

class CasinoHeistSilentSneakyStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit CasinoHeistSilentSneakyStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;
};

#endif // CASINOHEISTSILENTSNEAKYSTRATEGY_H
