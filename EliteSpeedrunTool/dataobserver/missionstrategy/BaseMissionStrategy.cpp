#include "BaseMissionStrategy.h"

BaseMissionStrategy::BaseMissionStrategy(QObject* parent)
    : QObject { parent }
{
}

BaseMissionStrategy::~BaseMissionStrategy()
{
    remove();
}

void BaseMissionStrategy::remove()
{
}
