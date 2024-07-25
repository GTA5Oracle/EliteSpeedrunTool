#pragma once

#include "BaseDataFetcher.h"
#include <QLabel>

class HeadshotFetcher : public BaseDataFetcher {
    Q_OBJECT
public:
    explicit HeadshotFetcher(QObject* parent = nullptr);

    QString getDisplayName() override;

    QString id() override;

    QVariant fetchData() override;

    void fetchDataAndUpdateLabel() override;

    DisplayInfoSubFunction getType() override;

private:
    QString getHeadshotPattern();
};
