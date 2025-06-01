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
        return QObject::tr("防火墙");
        break;
    case Timer:
        return QObject::tr("计时");
        break;
    case Act3Headshot:
        return QObject::tr("末日将至爆头");
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
    case Act3Headshot:
        return "Act3Headshot";
        break;
    case AutoTimer:
        return "AutoTimer";
        break;
    default:
        return "";
    }
};

QString defaultRtssOsdText(DisplayInfoSubFunction f)
{
    switch (f) {
    case Firewall:
        return "<P=0,70><C0=FFFF00><C0>Fw %1<C>";
        break;
    case Timer:
        return "<P=0,100><C0=FFFF00><S0=170><S1=-100><C0><S0>%1:%2.<S><S1>%3<S><C>";
        break;
    case Act3Headshot:
        return "<P=0,130><C0=FFFF00><S0=170><C0><S0>%1<S><C>";
        break;
    case AutoTimer:
        return "<P=0,100><C0=FFFF00><S0=170><S1=-100><C0><S0>%1:%2.<S><S1>%3<S><C>";
        break;
    default:
        return "%1";
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

QString DisplayInfoSubFunctionItem::rtssOsdText() const
{
    return mRtssOsdText;
}

void DisplayInfoSubFunctionItem::setRtssOsdText(const QString& newRtssOsdText)
{
    if (mRtssOsdText == newRtssOsdText)
        return;
    mRtssOsdText = newRtssOsdText;
    emit rtssOsdTextChanged();
}

bool DisplayInfoSubFunctionItem::rtssDisplay() const
{
    return mRtssDisplay;
}

void DisplayInfoSubFunctionItem::setRtssDisplay(bool newRtssDisplay)
{
    if (mRtssDisplay == newRtssDisplay)
        return;
    mRtssDisplay = newRtssDisplay;
    emit rtssDisplayChanged();
}
