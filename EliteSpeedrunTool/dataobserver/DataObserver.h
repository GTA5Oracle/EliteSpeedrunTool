#pragma once

#include "MemoryUtil.h"
#include "dataobserver/datafetcher/MissionHashFetcher.h"
#include "dataobserver/missionstrategy/BakerBadBeatStrategy.h"
#include "dataobserver/missionstrategy/BaseMissionStrategy.h"
#include "dataobserver/missionstrategy/CasinoHeistAggressiveStrategy.h"
#include "dataobserver/missionstrategy/CasinoHeistEndStageStrategy.h"
#include "dataobserver/missionstrategy/CasinoHeistSilentSneakyStrategy.h"
#include "dataobserver/missionstrategy/CasinoHeistTheBigConStrategy.h"
#include "dataobserver/missionstrategy/DoomsdayAct1Strategy.h"
#include "dataobserver/missionstrategy/DoomsdayAct2Strategy.h"
#include "dataobserver/missionstrategy/DoomsdayAct3Strategy.h"
#include "dataobserver/missionstrategy/EmptyStrategy.h"
#include "dataobserver/missionstrategy/FleecaJobStrategy.h"
#include "dataobserver/missionstrategy/HumaneLabsRaidStrategy.h"
#include "dataobserver/missionstrategy/LostMcRipStrategy.h"
#include "dataobserver/missionstrategy/PacificStandardStrategy.h"
#include "dataobserver/missionstrategy/PrisonBreakStationStrategy.h"
#include "dataobserver/missionstrategy/PrisonBreakStrategy.h"
#include "dataobserver/missionstrategy/PrisonBreakWetWorkStrategy.h"
#include "dataobserver/missionstrategy/SeriesAFundingStrategy.h"
#include <QObject>
#include <QTimer>

#define dataObserver (DataObserver::instance())

class DataObserver : public QObject {
    Q_OBJECT
public:
    explicit DataObserver(QObject* parent = nullptr);
    ~DataObserver();

    static DataObserver* instance();

    BaseMissionStrategy* getMissionStrategy() const;
    void setMissionStrategy(BaseMissionStrategy* newMissionStrategy);

    void startObserve();
    void stopObserve();
    void destruct(); // 析构整个DataObserver，这时会发出onXxxRemoved信号，让其他持有QLabel的控件取消持有

    QList<QLabel*> getDisplayLabels();

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
    bool destructed = false;

signals:
    void onLabelsAdded(QList<QLabel*>);
    void onDisplayLabelsAdded(QList<QLabel*>);
    void onLabelsRemoved(QList<QLabel*>);
    void onDisplayLabelsRemoved(QList<QLabel*>);
    void onStartObserve();
    void onStopObserve();
    void onMissionChanged();

public:
    BaseMissionStrategy* emptyStrategy = new EmptyStrategy(this);

    const QMap<const unsigned long long, BaseMissionStrategy*> missionStrategyMap = {
        qMakePair(MemoryUtil::hashDoomsdayAct1, new DoomsdayAct1Strategy(this)),
        qMakePair(MemoryUtil::hashDoomsdayAct2, new DoomsdayAct2Strategy(this)),
        qMakePair(MemoryUtil::hashDoomsdayAct3P1, new DoomsdayAct3Strategy(this)),
        qMakePair(MemoryUtil::hashDoomsdayAct3P2, new DoomsdayAct3Strategy(this)),
        qMakePair(MemoryUtil::hashFleecaJob, new FleecaJobStrategy(this)),
        qMakePair(MemoryUtil::hashPrisonBreakWetWork, new PrisonBreakStationStrategy(this)),
        qMakePair(MemoryUtil::hashPrisonBreakWetWork, new PrisonBreakWetWorkStrategy(this)),
        qMakePair(MemoryUtil::hashPrisonBreak, new PrisonBreakStrategy(this)),
        qMakePair(MemoryUtil::hashHumaneLabsRaid, new HumaneLabsRaidStrategy(this)),
        qMakePair(MemoryUtil::hashSeriesAFunding, new SeriesAFundingStrategy(this)),
        qMakePair(MemoryUtil::hashPacificStandardP1, new PacificStandardStrategy(this)),
        qMakePair(MemoryUtil::hashPacificStandardP2, new PacificStandardStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive1, new CasinoHeistAggressiveStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive2, new CasinoHeistAggressiveStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive3, new CasinoHeistAggressiveStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive4, new CasinoHeistAggressiveStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive5, new CasinoHeistAggressiveStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistSilentSneaky1, new CasinoHeistSilentSneakyStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistSilentSneaky2, new CasinoHeistSilentSneakyStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistSilentSneaky3, new CasinoHeistSilentSneakyStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistSilentSneaky4, new CasinoHeistSilentSneakyStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistTheBigCon1, new CasinoHeistTheBigConStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistTheBigCon2, new CasinoHeistTheBigConStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistTheBigCon3, new CasinoHeistTheBigConStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistTheBigCon4, new CasinoHeistTheBigConStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistEndStage, new CasinoHeistEndStageStrategy(this)),
        qMakePair(MemoryUtil::hashLostMcRip, new LostMcRipStrategy(this)),
        qMakePair(MemoryUtil::hashBakerBadBeat, new BakerBadBeatStrategy(this)),
    };
};
