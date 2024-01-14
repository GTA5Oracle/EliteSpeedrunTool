#ifndef RASHKOVSKYDAMAGEFETCHER_H
#define RASHKOVSKYDAMAGEFETCHER_H

#include "BaseDataFetcher.h"

class RashkovskyDamageFetcher : public BaseDataFetcher<float> {
    Q_OBJECT
public:
    explicit RashkovskyDamageFetcher(QObject* parent = nullptr);

    float fetchData() override;
};

#endif // RASHKOVSKYDAMAGEFETCHER_H
