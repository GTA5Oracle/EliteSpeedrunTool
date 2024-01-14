#pragma once

#include "MemoryUtil.h"
#include "dataobserver/datafetcher/MissionHashFetcher.h"
#include "dataobserver/missionstrategy/BakerBadBeatStrategy.h"
#include "dataobserver/missionstrategy/BaseMissionStrategy.h"
#include "dataobserver/missionstrategy/CasinoHeistAggressiveStrategy.h"
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
#include "dataobserver/missionstrategy/PrisonBreakStrategy.h"
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

signals:
    void onLabelsAdded(QList<QLabel*>);
    void onDisplayLabelsAdded(QList<QLabel*>);
    void onLabelsRemoved(QList<QLabel*>);
    void onDisplayLabelsRemoved(QList<QLabel*>);
    void onStartObserve();
    void onStopObserve();

public:
    BaseMissionStrategy* emptyStrategy = new EmptyStrategy(this);

    const QMap<const unsigned long long, BaseMissionStrategy*> missionStrategyMap = {
        qMakePair(MemoryUtil::hashDoomsdayAct1, new DoomsdayAct1Strategy(this)),
        qMakePair(MemoryUtil::hashDoomsdayAct2, new DoomsdayAct2Strategy(this)),
        qMakePair(MemoryUtil::hashDoomsdayAct3P1, doomsdayAct3Strategy),
        qMakePair(MemoryUtil::hashDoomsdayAct3P2, doomsdayAct3Strategy),
        qMakePair(MemoryUtil::hashFleecaJob, new FleecaJobStrategy(this)),
        qMakePair(MemoryUtil::hashPrisonBreak, new PrisonBreakStrategy(this)),
        qMakePair(MemoryUtil::hashHumaneLabsRaid, new HumaneLabsRaidStrategy(this)),
        qMakePair(MemoryUtil::hashSeriesAFunding, new SeriesAFundingStrategy(this)),
        qMakePair(MemoryUtil::hashPacificStandardP1, new PacificStandardStrategy(this)),
        qMakePair(MemoryUtil::hashPacificStandardP2, new PacificStandardStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistAggressive, new CasinoHeistAggressiveStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistSilentSneaky, new CasinoHeistSilentSneakyStrategy(this)),
        qMakePair(MemoryUtil::hashCasinoHeistTheBigCon, new CasinoHeistTheBigConStrategy(this)),
        qMakePair(MemoryUtil::hashLostMcRip, new LostMcRipStrategy(this)),
        qMakePair(MemoryUtil::hashBakerBadBeat, new BakerBadBeatStrategy(this)),
    };
};
