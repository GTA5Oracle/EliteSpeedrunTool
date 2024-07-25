#include "ThePrisonBreakStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

ThePrisonBreakStrategy::ThePrisonBreakStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString ThePrisonBreakStrategy::getDisplayName()
{
    return tr("越狱");
}

QSet<QString> ThePrisonBreakStrategy::defaultDataFetchers()
{
    return { dataFetcherUtil->rashkovskyDamageFetcher->id() };
}

QString ThePrisonBreakStrategy::id()
{
    return "ThePrisonBreak";
}

const QSet<unsigned long long> ThePrisonBreakStrategy::missionHash()
{
    return { MemoryUtil::hashThePrisonBreak };
}
