#include <QColor>
#include <QFont>
#include <QObject>
#include <QPointF>

#pragma once

enum DisplayInfoSubFunction {
    Firewall = 1,
    Headshot = 2,
    Timer = 3,
    AutoTimer = 4,
    VehicleDamage = 5,
    RashkovskyDamage = 6,
    Kill = 7,
    PlaneDamage = 8,
};

namespace DisplayInfoSubFunctionUtil {
QString toDisplayString(DisplayInfoSubFunction f);
QString toString(DisplayInfoSubFunction f);
}

class DisplayInfoSubFunctionItem : public QObject {
    Q_OBJECT

public:
    DisplayInfoSubFunctionItem(QObject* parent = nullptr);
    DisplayInfoSubFunctionItem(bool display, Qt::Alignment textAlignment, int textSize,
        QColor textColor, QColor textShadowColor, int textShadowBlurRadius,
        QPointF textShadowOffset, QString fontFamily);

    bool display() const;
    void setDisplay(bool newDisplay);

    const Qt::Alignment& textAlignment() const;
    void setTextAlignment(const Qt::Alignment& newTextAlignment);

    int textSize() const;
    void setTextSize(int newTextSize);

    const QColor& textColor() const;
    void setTextColor(const QColor& newTextColor);

    const QColor& textShadowColor() const;
    void setTextShadowColor(const QColor& newTextShadowColor);

    int textShadowBlurRadius() const;
    void setTextShadowBlurRadius(int newTextShadowBlurRadius);

    QPointF textShadowOffset() const;
    void setTextShadowOffset(QPointF newTextShadowOffset);

    const QString& fontFamily() const;
    void setFontFamily(const QString& newFontFamily);

signals:
    void displayChanged(bool newDisplay);
    void textAlignmentChanged(const Qt::Alignment& newTextAlignment);
    void textSizeChanged(int newTextSize);
    void textColorChanged(const QColor& newTextColor);
    void textShadowColorChanged(const QColor& newTextShadowColor);
    void textShadowBlurRadiusChanged(int newTextShadowBlurRadius);
    void textShadowOffsetChanged(QPointF newTextShadowOffset);
    void fontFamilyChanged(const QString& newFontFamily);

private:
    bool mDisplay = true;
    Qt::Alignment mTextAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    int mTextSize = 26;
    QColor mTextColor = Qt::yellow;
    QColor mTextShadowColor = Qt::black;
    int mTextShadowBlurRadius = 7;
    QPointF mTextShadowOffset = { 0, 0 };
    QString mFontFamily = QFont().family();

    Q_PROPERTY(bool display READ display WRITE setDisplay NOTIFY displayChanged)
    Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment NOTIFY textAlignmentChanged)
    Q_PROPERTY(int textSize READ textSize WRITE setTextSize NOTIFY textSizeChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor textShadowColor READ textShadowColor WRITE setTextShadowColor NOTIFY textShadowColorChanged)
    Q_PROPERTY(int textShadowBlurRadius READ textShadowBlurRadius WRITE setTextShadowBlurRadius NOTIFY textShadowBlurRadiusChanged)
    Q_PROPERTY(QPointF textShadowOffset READ textShadowOffset WRITE setTextShadowOffset NOTIFY textShadowOffsetChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
};
