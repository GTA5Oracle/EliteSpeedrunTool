#pragma once

#include "BaseDataFetcher.h"

class MissionHashFetcher : public BaseDataFetcher {
    Q_OBJECT
public:
    explicit MissionHashFetcher(QObject* parent = nullptr);

    QString getDisplayName() override;

    QString id() override;

    QVariant fetchData() override;

    void fetchDataAndUpdateLabel() override;

    DisplayInfoSubFunction getType() override;
};
