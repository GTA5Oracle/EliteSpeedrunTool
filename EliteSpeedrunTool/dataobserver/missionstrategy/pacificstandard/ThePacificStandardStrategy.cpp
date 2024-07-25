#include "ThePacificStandardStrategy.h"
#include "MemoryUtil.h"

ThePacificStandardStrategy::ThePacificStandardStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString ThePacificStandardStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事");
}

QSet<QString> ThePacificStandardStrategy::defaultDataFetchers()
{
    return {};
}

QString ThePacificStandardStrategy::id()
{
    return "ThePacificStandard";
}

const QSet<unsigned long long> ThePacificStandardStrategy::missionHash()
{
    return {
        MemoryUtil::hashThePacificStandardP1,
        MemoryUtil::hashThePacificStandardP2,
    };
}
