#include "BaseMissionStrategy.h"
#include <QGraphicsDropShadowEffect>

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

void BaseMissionStrategy::setLabelTextStyle(
    QLabel* label,
    const QColor& textColor,
    const QColor& textShadowColor,
    qreal textShadowBlurRadius,
    const QPointF& textShadowOffset)
{
    label->setStyleSheet(QString("color: %1;").arg(textColor.name()));

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(textShadowColor);
    timerEffect->setBlurRadius(textShadowBlurRadius);
    timerEffect->setOffset(textShadowOffset);
    label->setGraphicsEffect(timerEffect);
}

void BaseMissionStrategy::initGlobalDataConnects(QLabel* label, DisplayInfoSubFunctionItem* item)
{
    setLabelTextStyle(
        label,
        item->textColor(),
        item->textShadowColor(),
        item->textShadowBlurRadius(),
        item->textShadowOffset());
    label->setFont(QFont(item->fontFamily(), item->textSize()));
    label->setAlignment(item->textAlignment());

    connect(item, &DisplayInfoSubFunctionItem::displayChanged, this,
        [label](bool newDisplay) {
            label->setVisible(newDisplay);
        });

    connect(item, &DisplayInfoSubFunctionItem::textAlignmentChanged, this,
        [label](const Qt::Alignment& newTextAlignment) {
            label->setAlignment(newTextAlignment);
        });

    connect(item, &DisplayInfoSubFunctionItem::textSizeChanged, this,
        [label, item](int newTextSize) {
            label->setFont(QFont(item->fontFamily(), newTextSize));
        });

    connect(item, &DisplayInfoSubFunctionItem::textColorChanged, this,
        [this, label, item](const QColor& newTextColor) {
            setLabelTextStyle(
                label,
                newTextColor,
                item->textShadowColor(),
                item->textShadowBlurRadius(),
                item->textShadowOffset());
        });

    connect(item, &DisplayInfoSubFunctionItem::textShadowColorChanged, this,
        [this, label, item](const QColor& newTextShadowColor) {
            setLabelTextStyle(
                label,
                item->textColor(),
                newTextShadowColor,
                item->textShadowBlurRadius(),
                item->textShadowOffset());
        });

    connect(item, &DisplayInfoSubFunctionItem::textShadowBlurRadiusChanged, this,
        [this, label, item](int newTextShadowBlurRadius) {
            setLabelTextStyle(
                label,
                item->textColor(),
                item->textShadowColor(),
                newTextShadowBlurRadius,
                item->textShadowOffset());
        });

    connect(item, &DisplayInfoSubFunctionItem::textShadowOffsetChanged, this,
        [this, label, item](QPointF newTextShadowOffset) {
            setLabelTextStyle(
                label,
                item->textColor(),
                item->textShadowColor(),
                item->textShadowBlurRadius(),
                newTextShadowOffset);
        });

    connect(item, &DisplayInfoSubFunctionItem::fontFamilyChanged, this,
        [label, item](const QString& newFontFamily) {
            label->setFont(QFont(newFontFamily, item->textSize()));
        });
}
