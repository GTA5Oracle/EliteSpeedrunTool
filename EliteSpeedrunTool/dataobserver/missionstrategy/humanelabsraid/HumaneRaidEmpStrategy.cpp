#include "HumaneRaidEmpStrategy.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

HumaneRaidEmpStrategy::HumaneRaidEmpStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString HumaneRaidEmpStrategy::getDisplayName()
{
    return tr("突袭人道研究实验室：电磁脉冲装置");
}

QSet<QString> HumaneRaidEmpStrategy::defaultDataFetchers()
{
    return {
        dataFetcherUtil->headshotFetcher->id(),
        dataFetcherUtil->killFetcher->id(),
    };
}

QString HumaneRaidEmpStrategy::id()
{
    return "HumaneRaidEmp";
}

const QSet<unsigned long long> HumaneRaidEmpStrategy::missionHash()
{
    return { MemoryUtil::hashHumaneRaidEmp };
}
