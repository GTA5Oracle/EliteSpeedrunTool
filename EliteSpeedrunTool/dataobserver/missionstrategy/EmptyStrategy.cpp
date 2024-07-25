#include "EmptyStrategy.h"

EmptyStrategy::EmptyStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

const QString EmptyStrategy::getDisplayName()
{
    return tr("未收录的任务");
}

QSet<QString> EmptyStrategy::defaultDataFetchers()
{
    return {};
}

QString EmptyStrategy::id()
{
    return "Empty";
}

const QSet<unsigned long long> EmptyStrategy::missionHash()
{
    return {};
}
