#pragma once

#include "BaseDataFetcher.h"

class MissionHashFetcher : public BaseDataFetcher<unsigned long long> {
    Q_OBJECT
public:
    explicit MissionHashFetcher(QObject* parent = nullptr);

    unsigned long long fetchData() override;
};
