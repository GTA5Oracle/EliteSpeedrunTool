#pragma once

#include "dataobserver/datafetcher/MissionHashFetcher.h"
#include "dataobserver/missionstrategy/BaseMissionStrategy.h"
#include "dataobserver/missionstrategy/DoomsdayAct3Strategy.h"
#include <QObject>
#include <QTimer>
#define dataObserver (DataObserver::instance())

struct MissionInfo {
    BaseMissionStrategy* missionStrategy;
    QString displayName;
};

class DataObserver : public QObject {
    Q_OBJECT
public:
    explicit DataObserver(QObject* parent = nullptr);

    static DataObserver* instance();

    BaseMissionStrategy* getMissionStrategy() const;
    void setMissionStrategy(BaseMissionStrategy* newMissionStrategy);

    void startObserve();
    void stopObserve();

protected:
    void onTimeout();
    void onMissionHashTimeout();

private:
    QTimer* missionHashTimer = new QTimer(this);
    QTimer* timer = new QTimer(this);

    unsigned long long lastMissionHash = 0;
    MissionHashFetcher missionHashFetcher = MissionHashFetcher();
    BaseMissionStrategy* missionStrategy;

    bool isObserving = false;

signals:
    void onLabelsAdded(QList<QLabel*>);
    void onDisplayLabelsAdded(QList<QLabel*>);
    void onLabelsRemoved(QList<QLabel*>);
    void onDisplayLabelsRemoved(QList<QLabel*>);
    void onStartObserve();
    void onStopObserve();

public:
    const QMap<unsigned long long, MissionInfo> missionStrategyMap = {
        qMakePair(MemoryUtil::hashDoomsdayAct1, MissionInfo {}),
        qMakePair(MemoryUtil::hashDoomsdayAct2, MissionInfo {}),
        qMakePair(MemoryUtil::hashDoomsdayAct3P1, MissionInfo { doomsdayAct3Strategy, tr("末日将至") }),
        qMakePair(MemoryUtil::hashDoomsdayAct3P2, MissionInfo { doomsdayAct3Strategy, tr("末日将至") }),
        qMakePair(MemoryUtil::hashFleecaJob, MissionInfo {}),
        qMakePair(MemoryUtil::hashPrisonBreak, MissionInfo {}),
        qMakePair(MemoryUtil::hashHumaneLabsRaid, MissionInfo {}),
        qMakePair(MemoryUtil::hashSeriesAFunding, MissionInfo {}),
        qMakePair(MemoryUtil::hashPacificStandardP1, MissionInfo {}),
        qMakePair(MemoryUtil::hashPacificStandardP2, MissionInfo {}),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive, MissionInfo {}),
        qMakePair(MemoryUtil::hashCasinoHeistSilentSneaky, MissionInfo {}),
        qMakePair(MemoryUtil::hashCasinoHeistTheBigCon, MissionInfo {}),
    };
};
