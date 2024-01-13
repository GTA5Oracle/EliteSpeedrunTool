#ifndef EMPTYSTRATEGY_H
#define EMPTYSTRATEGY_H

#include "BaseMissionStrategy.h"

class EmptyStrategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit EmptyStrategy(QObject* parent = nullptr);

    QList<QLabel*> getDisplayLabels() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    QLabel labMissionName = QLabel(getDisplayName());
};

#endif // EMPTYSTRATEGY_H
