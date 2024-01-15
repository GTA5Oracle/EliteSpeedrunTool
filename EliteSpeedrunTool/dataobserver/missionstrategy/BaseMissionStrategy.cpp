#include "BaseMissionStrategy.h"
#include "GlobalData.h"
#include <QGraphicsDropShadowEffect>

BaseMissionStrategy::BaseMissionStrategy(QObject* parent)
    : QObject { parent }
{
    missionNameFont.setPointSize(14);
    font.setPointSize(12);
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
        if (!isGlobalDataConnectsInited) {
            isGlobalDataConnectsInited = true;
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

void BaseMissionStrategy::setLabelTextStyle(QLabel* label, DisplayInfoSubFunctionItem* item)
{
    label->setStyleSheet(QString("color: %1;").arg(item->textColor().name()));

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(item->textShadowColor());
    timerEffect->setBlurRadius(item->textShadowBlurRadius());
    timerEffect->setOffset(item->textShadowOffset());
    label->setGraphicsEffect(timerEffect);
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
