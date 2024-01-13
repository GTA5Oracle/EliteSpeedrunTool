#include "EmptyStrategy.h"

EmptyStrategy::EmptyStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
}

QList<QLabel*> EmptyStrategy::getDisplayLabels()
{
    return QList<QLabel*>();
}

QList<QLabel*> EmptyStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName;
}

void EmptyStrategy::updateInfo()
{
}

const QString EmptyStrategy::getDisplayName()
{
    return tr("不在支持的任务中...");
}
