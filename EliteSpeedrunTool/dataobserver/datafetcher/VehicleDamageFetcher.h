#ifndef VEHICLEDAMAGEFETCHER_H
#define VEHICLEDAMAGEFETCHER_H

#include "BaseDataFetcher.h"

#include <QLabel>

class VehicleDamageFetcher : public BaseDataFetcher {
    Q_OBJECT
public:
    explicit VehicleDamageFetcher(QObject* parent = nullptr);

    QString getDisplayName() override;

    QString id() override;

    QVariant fetchData() override;

    void fetchDataAndUpdateLabel() override;

    DisplayInfoSubFunction getType() override;

private:
    QString getVehicleDamagePattern();
};

#endif // VEHICLEDAMAGEFETCHER_H
