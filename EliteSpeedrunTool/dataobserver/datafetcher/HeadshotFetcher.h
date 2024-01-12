#pragma once

#include "BaseDataFetcher.h"

class HeadshotFetcher : public BaseDataFetcher<int> {
    Q_OBJECT
public:
    explicit HeadshotFetcher(QObject* parent = nullptr);

    int fetchData() override;
};
