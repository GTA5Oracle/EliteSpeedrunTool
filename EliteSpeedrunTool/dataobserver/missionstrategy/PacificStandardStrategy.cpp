#include "PacificStandardStrategy.h"

PacificStandardStrategy::PacificStandardStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
}

QList<QLabel*> PacificStandardStrategy::getDisplayLabels()
{
    return {};
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> PacificStandardStrategy::getDisplayLabelsAndItems()
{
    return {};
}

QList<QLabel*> PacificStandardStrategy::getLabels()
{
    return { &labMissionName };
}

void PacificStandardStrategy::updateInfo()
{
}

const QString PacificStandardStrategy::getDisplayName()
{
    return tr("太平洋标准银行差事");
}
