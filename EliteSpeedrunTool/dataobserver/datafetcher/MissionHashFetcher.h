#pragma once

#include "BaseDataFetcher.h"

class MissionHashFetcher : public BaseDataFetcher<unsigned int> {
    Q_OBJECT
public:
    explicit MissionHashFetcher(QObject* parent = nullptr);

    unsigned int fetchData() override;
};
