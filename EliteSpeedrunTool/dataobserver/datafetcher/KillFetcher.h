#pragma once

#include "BaseDataFetcher.h"
#include <QLabel>

class KillFetcher : public BaseDataFetcher {
    Q_OBJECT
public:
    explicit KillFetcher(QObject* parent = nullptr);

    QString getDisplayName() override;

    QString id() override;

    QVariant fetchData() override;

    void fetchDataAndUpdateLabel() override;

    DisplayInfoSubFunction getType() override;

private:
    QString getKillPattern();
};
