#include "BaseMissionStrategy.h"
#include "GlobalData.h"
#include <QGraphicsDropShadowEffect>

BaseMissionStrategy::BaseMissionStrategy(QObject* parent)
    : QObject { parent }
{
    initLabelFont();
}

BaseMissionStrategy::~BaseMissionStrategy()
{
    setCurrentStrategy(false);
}

void BaseMissionStrategy::setCurrentStrategy(bool isCurrent)
{
    this->isCurrent = isCurrent;
    if (isCurrent) {
        initSettings();
        if (!isLaterInited) {
            isLaterInited = true;
            labMissionName.setText(getDisplayName());
            initGlobalDataConnects();
        }
    }
}

bool BaseMissionStrategy::isCurrentStrategy()
{
    return isCurrent;
}

bool BaseMissionStrategy::labelIsVisible(QLabel* label)
{
    return labelVisibleMap[label];
}

void BaseMissionStrategy::initSettings()
{
    auto labelsAndItems = getDisplayLabelsAndItems();
    for (auto labelsAndItem : labelsAndItems) {
        setLabelDisplay(labelsAndItem.first, labelsAndItem.second);
        setLabelTextAlignment(labelsAndItem.first, labelsAndItem.second);
        setLabelFont(labelsAndItem.first, labelsAndItem.second);
        setLabelTextStyle(labelsAndItem.first, labelsAndItem.second);
    }
}

void BaseMissionStrategy::initLabelFont()
{
    missionNameFont.setPointSize(14);
    font.setPointSize(12);

    labMissionName.setFont(missionNameFont);

    labVehicleDamage.setFont(font);
    labPlaneDamage.setFont(font);
    labRashkovskyDamage.setFont(font);
    labHeadshot.setFont(font);
    labKill.setFont(font);
}

void BaseMissionStrategy::resetLabelData()
{
    updateKillLabel(0);
    updateHeadshotLabel(0);
    updateVehicleDamageLabel(0.0f);
    updatePlaneDamageLabel(0.0f);
    updateRashkovskyHurtLabel(0.0f);
}

void BaseMissionStrategy::setLabelTextStyle(QLabel* label, DisplayInfoSubFunctionItem* item)
{
    label->setStyleSheet(QString("color: %1;").arg(item->textColor().name()));

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(item->textShadowColor());
    timerEffect->setBlurRadius(item->textShadowBlurRadius());
    timerEffect->setOffset(item->textShadowOffset());
    label->setGraphicsEffect(timerEffect);
}

QString BaseMissionStrategy::getKillPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "💀%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return tr("总击杀：%1");
    }
}

QString BaseMissionStrategy::getHeadshotPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "☠️%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return tr("总爆头：%1");
    }
}

QString BaseMissionStrategy::getVehicleDamagePattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "🚗%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return tr("车损：%1");
    }
}

QString BaseMissionStrategy::getPlaneDamagePattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "🚁%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return tr("机损：%1");
    }
}

QString BaseMissionStrategy::getRashkovskyHurtPattern()
{
    switch (globalData->missionDataName()) {
    case MissionDataNameUtil::MissionDataName::Emoji:
        return "👨🏻‍🦲%1";
    case MissionDataNameUtil::MissionDataName::None:
        return "%1";
    default:
        return tr("拉什科夫斯基受伤：%1");
    }
}

void BaseMissionStrategy::updateKillLabel(int newValue)
{
    static QVariant lastValue;
    // 没有parent，setText无效
    if (!labKill.parentWidget() && !labDisplayKill.parentWidget()) {
        lastValue.clear();
        return;
    }
    // 若跟上次更新的数据一样则直接跳过
    if (!lastValue.isNull() && lastValue.toInt() == newValue) {
        return;
    }
    lastValue.setValue(newValue);

    auto text = getKillPattern().arg(QString::number(newValue));
    labDisplayKill.setText(text);
    labKill.setText(text);
}

void BaseMissionStrategy::updateHeadshotLabel(int newValue)
{
    static QVariant lastValue;
    // 没有parent，setText无效
    if (!labHeadshot.parentWidget() && !labDisplayHeadshot.parentWidget()) {
        lastValue.clear();
        return;
    }
    // 若跟上次更新的数据一样则直接跳过
    if (!lastValue.isNull() && lastValue.toInt() == newValue) {
        return;
    }

    lastValue.setValue(newValue);

    auto text = getHeadshotPattern().arg(QString::number(newValue));
    labDisplayHeadshot.setText(text);
    labHeadshot.setText(text);
}

void BaseMissionStrategy::updateVehicleDamageLabel(float newValue)
{
    static QVariant lastValue;
    // 没有parent，setText无效
    if (!labVehicleDamage.parentWidget() && !labDisplayVehicleDamage.parentWidget()) {
        lastValue.clear();
        return;
    }
    // 若跟上次更新的数据一样则直接跳过
    if (!lastValue.isNull() && lastValue.toFloat() == newValue) {
        return;
    }
    lastValue.setValue(newValue);

    auto text = getVehicleDamagePattern().arg(QString::number(newValue, 'f', 2));
    labDisplayVehicleDamage.setText(text);
    labVehicleDamage.setText(text);
}

void BaseMissionStrategy::updatePlaneDamageLabel(float newValue)
{
    static QVariant lastValue;
    // 没有parent，setText无效
    if (!labPlaneDamage.parentWidget() && !labDisplayPlaneDamage.parentWidget()) {
        lastValue.clear();
        return;
    }
    // 若跟上次更新的数据一样则直接跳过
    if (!lastValue.isNull() && lastValue.toFloat() == newValue) {
        return;
    }
    lastValue.setValue(newValue);

    auto text = getPlaneDamagePattern().arg(QString::number(newValue, 'f', 2));
    labDisplayPlaneDamage.setText(text);
    labPlaneDamage.setText(text);
}

void BaseMissionStrategy::updateRashkovskyHurtLabel(float newValue)
{
    static QVariant lastValue;
    // 没有parent，setText无效
    if (!labRashkovskyDamage.parentWidget() && !labDisplayRashkovskyDamage.parentWidget()) {
        lastValue.clear();
        return;
    }
    // 若跟上次更新的数据一样则直接跳过
    if (!lastValue.isNull() && lastValue.toFloat() == newValue) {
        return;
    }
    lastValue.setValue(newValue);

    auto text = getRashkovskyHurtPattern().arg(QString::number(newValue, 'f', 2));
    labDisplayRashkovskyDamage.setText(text);
    labRashkovskyDamage.setText(text);
}

void BaseMissionStrategy::initGlobalDataConnects()
{
    auto labelsAndItems = getDisplayLabelsAndItems();
    for (auto labelsAndItem : labelsAndItems) {
        initGlobalDataConnects(labelsAndItem.first, labelsAndItem.second);
    }
}

void BaseMissionStrategy::setLabelDisplay(QLabel* label, DisplayInfoSubFunctionItem* item)
{
    labelVisibleMap[label] = item->display();
    if (isCurrentStrategy() && globalData->displayInfoShow()) {
        label->setVisible(item->display());
    }
}

void BaseMissionStrategy::setLabelTextAlignment(QLabel* label, DisplayInfoSubFunctionItem* item)
{
    label->setAlignment(item->textAlignment());
}

void BaseMissionStrategy::setLabelFont(QLabel* label, DisplayInfoSubFunctionItem* item)
{
    label->setFont(QFont(item->fontFamily(), item->textSize()));
}

void BaseMissionStrategy::initGlobalDataConnects(QLabel* label, DisplayInfoSubFunctionItem* item)
{
    connect(item, &DisplayInfoSubFunctionItem::displayChanged, this,
        [this, label, item](bool newDisplay) {
            setLabelDisplay(label, item);
        });

    connect(item, &DisplayInfoSubFunctionItem::textAlignmentChanged, this,
        [this, label, item](const Qt::Alignment& newTextAlignment) {
            setLabelTextAlignment(label, item);
        });

    connect(item, &DisplayInfoSubFunctionItem::textSizeChanged, this,
        [this, label, item](int newTextSize) {
            setLabelFont(label, item);
        });

    connect(item, &DisplayInfoSubFunctionItem::textColorChanged, this,
        [this, label, item](const QColor& newTextColor) {
            setLabelTextStyle(label, item);
        });

    connect(item, &DisplayInfoSubFunctionItem::textShadowColorChanged, this,
        [this, label, item](const QColor& newTextShadowColor) {
            setLabelTextStyle(label, item);
        });

    connect(item, &DisplayInfoSubFunctionItem::textShadowBlurRadiusChanged, this,
        [this, label, item](int newTextShadowBlurRadius) {
            setLabelTextStyle(label, item);
        });

    connect(item, &DisplayInfoSubFunctionItem::textShadowOffsetChanged, this,
        [this, label, item](QPointF newTextShadowOffset) {
            setLabelTextStyle(label, item);
        });

    connect(item, &DisplayInfoSubFunctionItem::fontFamilyChanged, this,
        [this, label, item](const QString& newFontFamily) {
            setLabelFont(label, item);
        });
}
