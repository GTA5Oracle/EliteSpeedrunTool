#include "PrisonBreakStrategy.h"
#include "GlobalData.h"

PrisonBreakStrategy::PrisonBreakStrategy(QObject* parent)
    : BaseMissionStrategy { parent }
{
    labMissionName.setFont(missionNameFont);
    labRashkovskyDamage.setFont(font);

    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    initGlobalDataConnects(&labDisplayRashkovskyDamage, displayInfoSubFunctions[DisplayInfoSubFunction::RashkovskyDamage]);
}

QList<QLabel*> PrisonBreakStrategy::getDisplayLabels()
{
    return QList<QLabel*>() << &labDisplayRashkovskyDamage;
}

QList<QLabel*> PrisonBreakStrategy::getLabels()
{
    return QList<QLabel*>() << &labMissionName << &labRashkovskyDamage;
}

void PrisonBreakStrategy::updateInfo()
{
    auto data = rashkovskyDamageFetcher.fetchData();
    auto text = rashkovskyDamagePattern.arg(QString::number(data, 'f', 2));
    labDisplayRashkovskyDamage.setText(text);
    labRashkovskyDamage.setText(text);
}

const QString PrisonBreakStrategy::getDisplayName()
{
    return tr("越狱");
}
