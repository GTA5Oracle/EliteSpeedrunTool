#include "EmptyStrategy.h"

EmptyStrategy::EmptyStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> EmptyStrategy::getDisplayLabels()
{
    return {};
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> EmptyStrategy::getDisplayLabelsAndItems()
{
    return {};
}

QList<QLabel*> EmptyStrategy::getLabels()
{
    return { &labMissionName };
}

void EmptyStrategy::updateInfo()
{
}

const QString EmptyStrategy::getDisplayName()
{
    return tr("不在支持的任务中...");
}
