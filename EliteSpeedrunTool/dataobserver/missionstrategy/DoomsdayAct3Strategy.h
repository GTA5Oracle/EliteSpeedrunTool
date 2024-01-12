#pragma once

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/HeadshotFetcher.h"
#define doomsdayAct3Strategy (DoomsdayAct3Strategy::instance())

class DoomsdayAct3Strategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit DoomsdayAct3Strategy(QObject* parent = nullptr);
    ~DoomsdayAct3Strategy();

    static DoomsdayAct3Strategy* instance();

    QList<QLabel*> getDisplayLabels() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;

private:
    HeadshotFetcher headshotFetcher = HeadshotFetcher();

    QLabel* labDisplayHeadshot = new QLabel();
    QLabel* labHeadshot = new QLabel();
};
