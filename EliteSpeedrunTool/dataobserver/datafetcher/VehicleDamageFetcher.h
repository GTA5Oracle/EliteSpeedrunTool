#ifndef VEHICLEDAMAGEFETCHER_H
#define VEHICLEDAMAGEFETCHER_H

#include "BaseDataFetcher.h"

class VehicleDamageFetcher : public BaseDataFetcher<float> {
    Q_OBJECT
public:
    explicit VehicleDamageFetcher(QObject* parent = nullptr);

    float fetchData() override;
};

#endif // VEHICLEDAMAGEFETCHER_H
