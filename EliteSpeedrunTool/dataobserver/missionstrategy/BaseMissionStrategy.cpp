#include "BaseMissionStrategy.h"

BaseMissionStrategy::BaseMissionStrategy(QObject* parent)
    : QObject { parent }
{
    missionNameFont.setPointSize(14);
    font.setPointSize(12);
}

BaseMissionStrategy::~BaseMissionStrategy()
{
    remove();
}

void BaseMissionStrategy::remove()
{
}
