#include "TheBogdanProblemStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

TheBogdanProblemStrategy::TheBogdanProblemStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString TheBogdanProblemStrategy::getDisplayName()
{
    return tr("波格丹危机");
}

QSet<QString> TheBogdanProblemStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->vehicleDamageFetcher->id() };
}

QString TheBogdanProblemStrategy::id()
{
    return "TheBogdanProblem";
}

const QSet<unsigned long long> TheBogdanProblemStrategy::missionHash()
{
    return { MemoryUtil::hashTheBogdanProblem };
}
