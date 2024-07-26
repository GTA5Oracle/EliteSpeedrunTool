#ifndef MISSIONSTRATEGYUTIL_H
#define MISSIONSTRATEGYUTIL_H

#include "BaseMissionStrategy.h"
#include "EmptyStrategy.h"
#include "LostMcRipStrategy.h"
#include "act1/Act1DeadCourierStrategy.h"
#include "act1/Act1ServerFarmStrategy.h"
#include "act1/Act1SignalInterceptsStrategy.h"
#include "act1/TheDataBreachesStrategy.h"
#include "act2/Act2AvengerStrategy.h"
#include "act2/Act2RescueUlpStrategy.h"
#include "act2/Act2SalvageHardDrivesStrategy.h"
#include "act2/Act2SubmarineReconStrategy.h"
#include "act2/TheBogdanProblemStrategy.h"
#include "act3/Act3AirDefensesStrategy.h"
#include "act3/Act3BarrageStrategy.h"
#include "act3/Act3EscortUlpStrategy.h"
#include "act3/Act3KhanjaliStrategy.h"
#include "act3/Act3RescueAgent14Strategy.h"
#include "act3/TheDoomsdayScenarioStrategy.h"
#include "casino/CasinoBadBeatStrategy.h"
#include "casinoheist/CasinoHeistAggressiveStrategy.h"
#include "casinoheist/CasinoHeistEndStageStrategy.h"
#include "casinoheist/CasinoHeistSilentSneakyStrategy.h"
#include "casinoheist/CasinoHeistTheBigConStrategy.h"
#include "fleecajob/FleecaJobKurumaStrategy.h"
#include "fleecajob/FleecaJobScopeOutStrategy.h"
#include "fleecajob/TheFleecaJobStrategy.h"
#include "humanelabsraid/HumaneRaidInsurgentsStrategy.h"
#include "humanelabsraid/HumaneRaidValkyrieStrategy.h"
#include "humanelabsraid/TheHumaneLabsRaidStrategy.h"
#include "pacificstandard/ThePacificStandardStrategy.h"
#include "prisonbreak/PrisonBreakBusStrategy.h"
#include "prisonbreak/PrisonBreakStationStrategy.h"
#include "prisonbreak/PrisonBreakWetWorkStrategy.h"
#include "prisonbreak/ThePrisonBreakStrategy.h"
#include "seriesa/SeriesAFundingStrategy.h"
#include <QObject>

#define missionStrategyUtil (MissionStrategyUtil::instance())

class MissionStrategyUtil : public QObject {
    Q_OBJECT
public:
    explicit MissionStrategyUtil(QObject* parent = nullptr);

    static MissionStrategyUtil* instance();

    void readSettings();
    void writeSettings();

    EmptyStrategy* emptyStrategy = new EmptyStrategy(this);

    TheDataBreachesStrategy* theDataBreachesStrategy = new TheDataBreachesStrategy(this);
    Act1DeadCourierStrategy* act1DeadCourierStrategy = new Act1DeadCourierStrategy(this);
    Act1SignalInterceptsStrategy* act1SignalInterceptsStrategy = new Act1SignalInterceptsStrategy(this);
    Act1ServerFarmStrategy* act1ServerFarmStrategy = new Act1ServerFarmStrategy(this);

    TheBogdanProblemStrategy* theBogdanProblemStrategy = new TheBogdanProblemStrategy(this);
    Act2AvengerStrategy* act2AvengerStrategy = new Act2AvengerStrategy(this);
    Act2RescueUlpStrategy* act2RescueUlpStrategy = new Act2RescueUlpStrategy(this);
    Act2SalvageHardDrivesStrategy* act2SalvageHardDrivesStrategy = new Act2SalvageHardDrivesStrategy(this);
    Act2SubmarineReconStrategy* act2SubmarineReconStrategy = new Act2SubmarineReconStrategy(this);

    TheDoomsdayScenarioStrategy* theDoomsdayScenarioStrategy = new TheDoomsdayScenarioStrategy(this);
    Act3RescueAgent14Strategy* act3RescueAgent14Strategy = new Act3RescueAgent14Strategy(this);
    Act3EscortUlpStrategy* act3EscortUlpStrategy = new Act3EscortUlpStrategy(this);
    Act3BarrageStrategy* act3BarrageStrategy = new Act3BarrageStrategy(this);
    Act3KhanjaliStrategy* act3KhanjaliStrategy = new Act3KhanjaliStrategy(this);
    Act3AirDefensesStrategy* act3AirDefensesStrategy = new Act3AirDefensesStrategy(this);

    FleecaJobScopeOutStrategy* fleecaJobScopeOutStrategy = new FleecaJobScopeOutStrategy(this);
    FleecaJobKurumaStrategy* fleecaJobKurumaStrategy = new FleecaJobKurumaStrategy(this);
    TheFleecaJobStrategy* theFleecaJobStrategy = new TheFleecaJobStrategy(this);

    PrisonBreakBusStrategy* prisonBreakBusStrategy = new PrisonBreakBusStrategy(this);
    PrisonBreakStationStrategy* prisonBreakStationStrategy = new PrisonBreakStationStrategy(this);
    PrisonBreakWetWorkStrategy* prisonBreakWetWorkStrategy = new PrisonBreakWetWorkStrategy(this);
    ThePrisonBreakStrategy* thePrisonBreakStrategy = new ThePrisonBreakStrategy(this);

    HumaneRaidInsurgentsStrategy* humaneRaidInsurgentsStrategy = new HumaneRaidInsurgentsStrategy(this);
    HumaneRaidValkyrieStrategy* humaneRaidValkyrieStrategy = new HumaneRaidValkyrieStrategy(this);
    TheHumaneLabsRaidStrategy* theHumaneLabsRaidStrategy = new TheHumaneLabsRaidStrategy(this);

    SeriesAFundingStrategy* seriesAFundingStrategy = new SeriesAFundingStrategy(this);

    ThePacificStandardStrategy* thePacificStandardStrategy = new ThePacificStandardStrategy(this);

    CasinoHeistAggressiveStrategy* casinoHeistAggressiveStrategy = new CasinoHeistAggressiveStrategy(this);
    CasinoHeistSilentSneakyStrategy* casinoHeistSilentSneakyStrategy = new CasinoHeistSilentSneakyStrategy(this);
    CasinoHeistTheBigConStrategy* casinoHeistTheBigConStrategy = new CasinoHeistTheBigConStrategy(this);
    CasinoHeistEndStageStrategy* casinoHeistEndStageStrategy = new CasinoHeistEndStageStrategy(this);

    LostMcRipStrategy* lostMcRipStrategy = new LostMcRipStrategy(this);
    CasinoBadBeatStrategy* casinoBadBeatStrategy = new CasinoBadBeatStrategy(this);

    QList<BaseMissionStrategy*> missionStrategies = {
        act1DeadCourierStrategy,
        act1SignalInterceptsStrategy,
        act1ServerFarmStrategy,
        theDataBreachesStrategy,

        act2AvengerStrategy,
        act2RescueUlpStrategy,
        act2SalvageHardDrivesStrategy,
        act2SubmarineReconStrategy,
        theBogdanProblemStrategy,

        act3RescueAgent14Strategy,
        act3EscortUlpStrategy,
        act3BarrageStrategy,
        act3KhanjaliStrategy,
        act3AirDefensesStrategy,
        theDoomsdayScenarioStrategy,

        fleecaJobScopeOutStrategy,
        fleecaJobKurumaStrategy,
        theFleecaJobStrategy,

        prisonBreakBusStrategy,
        prisonBreakStationStrategy,
        prisonBreakWetWorkStrategy,
        thePrisonBreakStrategy,

        humaneRaidInsurgentsStrategy,
        humaneRaidValkyrieStrategy,
        theHumaneLabsRaidStrategy,

        seriesAFundingStrategy,

        thePacificStandardStrategy,

        casinoHeistAggressiveStrategy,
        casinoHeistSilentSneakyStrategy,
        casinoHeistTheBigConStrategy,
        casinoHeistEndStageStrategy,

        lostMcRipStrategy,

        casinoBadBeatStrategy
    };

    QMap<QString, BaseMissionStrategy*> missionIdToObject;

    const QMap<QString, QSet<QString>>& missionToDataFetchers() const;

    void updateMissionToDataFetchers(QString missionId, QSet<QString> fetchers);

    void insertToMissionToDataFetchers(QString missionId, QString fetcherId);

    void removeFromMissionToDataFetchers(QString missionId, QString fetcherId);

private:
    QMap<QString, QSet<QString>> mMissionToDataFetchers;

signals:
    void onMissionToDataFetchersChanged(QString, QSet<QString> oldFetchers, QSet<QString> newFetchers);
};

#endif // MISSIONSTRATEGYUTIL_H
