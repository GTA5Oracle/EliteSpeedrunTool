#include "PacificStandardStrategy.h"

PacificStandardStrategy::PacificStandardStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
}

QList<QLabel*> PacificStandardStrategy::getDisplayLabels()
{
    return QList<QLabel*>();
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> PacificStandardStrategy::getDisplayLabelsAndItems()
{
    return {};
}

QList<QLabel*> PacificStandardStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName;
}

void PacificStandardStrategy::updateInfo()
{
}

const QString PacificStandardStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事");
}
