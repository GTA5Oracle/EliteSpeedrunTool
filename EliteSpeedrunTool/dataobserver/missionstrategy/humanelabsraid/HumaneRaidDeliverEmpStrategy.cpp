#include "HumaneRaidDeliverEmpStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

HumaneRaidDeliverEmpStrategy::HumaneRaidDeliverEmpStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString HumaneRaidDeliverEmpStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室：运送电磁脉冲装置");
}

QSet<QString> HumaneRaidDeliverEmpStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString HumaneRaidDeliverEmpStrategy::id()
{
    return "HumaneRaidDeliverEmp";
}

const QSet<unsigned long long> HumaneRaidDeliverEmpStrategy::missionHash()
{
    return { MemoryUtil::hashHumaneRaidDeliverEmp };
}
