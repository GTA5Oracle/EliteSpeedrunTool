#include "DisplayInfoSubFunction.h"
#include <QColor>
#include <QFont>
#include <QObject>
#include <QPointF>

namespace DisplayInfoSubFunctionUtil {

QString toDisplayString(DisplayInfoSubFunction f)
{
    switch (f) {
    case Firewall:
        return QObject::tr("断网");
        break;
    case Timer:
        return QObject::tr("计时");
        break;
    default:
        return "";
    }
};

QString toString(DisplayInfoSubFunction f)
{
    switch (f) {
    case Firewall:
        return "Firewall";
        break;
    case Timer:
        return "Timer";
        break;
    default:
        return "";
    }
};
}

DisplayInfoSubFunctionItem::DisplayInfoSubFunctionItem(QObject* parent)
    : QObject(parent)
{
}

DisplayInfoSubFunctionItem::DisplayInfoSubFunctionItem(bool display, Qt::Alignment textAlignment, int textSize, QColor textColor, QColor textShadowColor, int textShadowBlurRadius, QPointF textShadowOffset, QString fontFamily)
{
    this->mDisplay = display;
    this->mTextAlignment = textAlignment;
    this->mTextAlignment = textAlignment;
    this->mTextSize = textSize;
    this->mTextColor = textColor;
    this->mTextShadowColor = textShadowColor;
    this->mTextShadowBlurRadius = textShadowBlurRadius;
    this->mTextShadowOffset = textShadowOffset;
    this->mFontFamily = fontFamily;
}

bool DisplayInfoSubFunctionItem::display() const
{
    return mDisplay;
}

void DisplayInfoSubFunctionItem::setDisplay(bool newDisplay)
{
    if (mDisplay == newDisplay)
        return;
    mDisplay = newDisplay;
    emit displayChanged(mDisplay);
}

const Qt::Alignment& DisplayInfoSubFunctionItem::textAlignment() const
{
    return mTextAlignment;
}

void DisplayInfoSubFunctionItem::setTextAlignment(const Qt::Alignment& newTextAlignment)
{
    if (mTextAlignment == newTextAlignment)
        return;
    mTextAlignment = newTextAlignment;
    emit textAlignmentChanged(mTextAlignment);
}

int DisplayInfoSubFunctionItem::textSize() const
{
    return mTextSize;
}

void DisplayInfoSubFunctionItem::setTextSize(int newTextSize)
{
    if (mTextSize == newTextSize)
        return;
    mTextSize = newTextSize;
    emit textSizeChanged(mTextSize);
}

const QColor& DisplayInfoSubFunctionItem::textColor() const
{
    return mTextColor;
}

void DisplayInfoSubFunctionItem::setTextColor(const QColor& newTextColor)
{
    if (mTextColor == newTextColor)
        return;
    mTextColor = newTextColor;
    emit textColorChanged(mTextColor);
}

const QColor& DisplayInfoSubFunctionItem::textShadowColor() const
{
    return mTextShadowColor;
}

void DisplayInfoSubFunctionItem::setTextShadowColor(const QColor& newTextShadowColor)
{
    if (mTextShadowColor == newTextShadowColor)
        return;
    mTextShadowColor = newTextShadowColor;
    emit textShadowColorChanged(mTextShadowColor);
}

int DisplayInfoSubFunctionItem::textShadowBlurRadius() const
{
    return mTextShadowBlurRadius;
}

void DisplayInfoSubFunctionItem::setTextShadowBlurRadius(int newTextShadowBlurRadius)
{
    if (mTextShadowBlurRadius == newTextShadowBlurRadius)
        return;
    mTextShadowBlurRadius = newTextShadowBlurRadius;
    emit textShadowBlurRadiusChanged(mTextShadowBlurRadius);
}

QPointF DisplayInfoSubFunctionItem::textShadowOffset() const
{
    return mTextShadowOffset;
}

void DisplayInfoSubFunctionItem::setTextShadowOffset(QPointF newTextShadowOffset)
{
    if (mTextShadowOffset == newTextShadowOffset)
        return;
    mTextShadowOffset = newTextShadowOffset;
    emit textShadowOffsetChanged(mTextShadowOffset);
}

const QString& DisplayInfoSubFunctionItem::fontFamily() const
{
    return mFontFamily;
}

void DisplayInfoSubFunctionItem::setFontFamily(const QString& newFontFamily)
{
    if (mFontFamily == newFontFamily)
        return;
    mFontFamily = newFontFamily;
    emit fontFamilyChanged(mFontFamily);
}
