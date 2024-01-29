#ifndef PACIFICSTANDARDSTRATEGY_H
#define PACIFICSTANDARDSTRATEGY_H

#include "BaseMissionStrategy.h"

class PacificStandardStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PacificStandardStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;
};

#endif // PACIFICSTANDARDSTRATEGY_H
