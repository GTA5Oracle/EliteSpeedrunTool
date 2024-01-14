#pragma once

#include "BaseMissionStrategy.h"
#include "dataobserver/datafetcher/HeadshotFetcher.h"
#define doomsdayAct3Strategy (DoomsdayAct3Strategy::instance())

class DoomsdayAct3Strategy : public BaseMissionStrategy {
    Q_OBJECT
public:
    explicit DoomsdayAct3Strategy(QObject* parent = nullptr);

    static DoomsdayAct3Strategy* instance();

    QList<QLabel*> getDisplayLabels() override;
    QList<QLabel*> getLabels() override;
    void updateInfo() override;
    const QString getDisplayName() override;

private:
    HeadshotFetcher headshotFetcher = HeadshotFetcher();

    QLabel labMissionName = QLabel(getDisplayName());

    QLabel labDisplayHeadshot = QLabel();
    QLabel labHeadshot = QLabel();
};
