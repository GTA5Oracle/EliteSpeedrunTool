#include "TheDoomsdayScenarioStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

TheDoomsdayScenarioStrategy::TheDoomsdayScenarioStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString TheDoomsdayScenarioStrategy::getDisplayName()
{
    return tr("末日将至");
}

QSet<QString> TheDoomsdayScenarioStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->headshotFetcher->id() };
}

QString TheDoomsdayScenarioStrategy::id()
{
    return "TheDoomsdayScenario";
}

const QSet<unsigned long long> TheDoomsdayScenarioStrategy::missionHash()
{
    return {
        MemoryUtil::hashTheDoomsdayScenarioP1,
        MemoryUtil::hashTheDoomsdayScenarioP2,
    };
}
