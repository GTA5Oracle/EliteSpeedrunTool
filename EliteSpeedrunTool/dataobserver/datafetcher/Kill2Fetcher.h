#pragma once

#include "BaseDataFetcher.h"
#include <QLabel>

class Kill2Fetcher : public BaseDataFetcher {
    Q_OBJECT
public:
    explicit Kill2Fetcher(QObject* parent = nullptr);

    QString getDisplayName() override;

    QString id() override;

    QVariant fetchData() override;

    void fetchDataAndUpdateLabel() override;

    DisplayInfoSubFunction getType() override;

private:
    QString getKillPattern();
};
