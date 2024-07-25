#ifndef RASHKOVSKYDAMAGEFETCHER_H
#define RASHKOVSKYDAMAGEFETCHER_H

#include "BaseDataFetcher.h"

#include <QLabel>

class RashkovskyDamageFetcher : public BaseDataFetcher {
    Q_OBJECT
public:
    explicit RashkovskyDamageFetcher(QObject* parent = nullptr);

    QString getDisplayName() override;

    QString id() override;

    QVariant fetchData() override;

    void fetchDataAndUpdateLabel() override;

    DisplayInfoSubFunction getType() override;

private:
    QString getRashkovskyHurtPattern();
};

#endif // RASHKOVSKYDAMAGEFETCHER_H
