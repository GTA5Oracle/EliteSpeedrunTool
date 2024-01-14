#ifndef PRISONBREAKSTRATEGY_H
#define PRISONBREAKSTRATEGY_H

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/RashkovskyDamageFetcher.h"

class PrisonBreakStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit PrisonBreakStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    RashkovskyDamageFetcher rashkovskyDamageFetcher = RashkovskyDamageFetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayRashkovskyDamage = QLabel();
    QLabel labRashkovskyDamage = QLabel();
};

#endif // PRISONBREAKSTRATEGY_H