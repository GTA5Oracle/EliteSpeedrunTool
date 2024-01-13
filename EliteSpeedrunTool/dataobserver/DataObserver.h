#pragma once

#include "MemoryUtil.h"
#include "dataobserver/datafetcher/MissionHashFetcher.h"
#include "dataobserver/missionstrategy/BaseMissionStrategy.h"
#include "dataobserver/missionstrategy/DoomsdayAct3Strategy.h"
#include "dataobserver/missionstrategy/EmptyStrategy.h"
#include "dataobserver/missionstrategy/FleecaJobStrategy.h"
#include "dataobserver/missionstrategy/LostMcRipStrategy.h"
#include <QObject>
#include <QTimer>
#define dataObserver (DataObserver::instance())

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
    BaseMissionStrategy* emptyStrategy = new EmptyStrategy();

    const QMap<const unsigned long long, BaseMissionStrategy*> missionStrategyMap = {
        qMakePair(MemoryUtil::hashDoomsdayAct1, nullptr),
        qMakePair(MemoryUtil::hashDoomsdayAct2, nullptr),
        qMakePair(MemoryUtil::hashDoomsdayAct3P1, doomsdayAct3Strategy),
        qMakePair(MemoryUtil::hashDoomsdayAct3P2, doomsdayAct3Strategy),
        qMakePair(MemoryUtil::hashFleecaJob, new FleecaJobStrategy()),
        qMakePair(MemoryUtil::hashPrisonBreak, nullptr),
        qMakePair(MemoryUtil::hashHumaneLabsRaid, nullptr),
        qMakePair(MemoryUtil::hashSeriesAFunding, nullptr),
        qMakePair(MemoryUtil::hashPacificStandardP1, nullptr),
        qMakePair(MemoryUtil::hashPacificStandardP2, nullptr),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive, nullptr),
        qMakePair(MemoryUtil::hashCasinoHeistSilentSneaky, nullptr),
        qMakePair(MemoryUtil::hashCasinoHeistTheBigCon, nullptr),
        qMakePair(MemoryUtil::hashLostMcRip, new LostMcRipStrategy()),
    };
};
