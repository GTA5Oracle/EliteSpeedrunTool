#ifndef PRISONBREAKSTRATEGY_H
#define PRISONBREAKSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/RashkovskyDamageFetcher.h"

class PrisonBreakStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PrisonBreakStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    RashkovskyDamageFetcher rashkovskyDamageFetcher = RashkovskyDamageFetcher();
};

#endif // PRISONBREAKSTRATEGY_H
