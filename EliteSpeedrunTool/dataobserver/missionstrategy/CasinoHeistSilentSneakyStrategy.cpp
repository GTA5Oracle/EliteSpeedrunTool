#include "CasinoHeistSilentSneakyStrategy.h"

CasinoHeistSilentSneakyStrategy::CasinoHeistSilentSneakyStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
}

QList<QLabel*> CasinoHeistSilentSneakyStrategy::getDisplayLabels()
{
    return {};
}

QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> CasinoHeistSilentSneakyStrategy::getDisplayLabelsAndItems()
{
    return {};
}

QList<QLabel*> CasinoHeistSilentSneakyStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName;
}

void CasinoHeistSilentSneakyStrategy::updateInfo()
{
}

const QString CasinoHeistSilentSneakyStrategy::getDisplayName()
{
    return tr("名钻赌场豪劫：隐迹潜踪");
}
