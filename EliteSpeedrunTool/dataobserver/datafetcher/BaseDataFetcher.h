#pragma once

#include "DisplayInfoSubFunction.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QLayout>
#include <QVariant>

class BaseDataFetcher : public QObject {
protected:
    bool inited = false;
    bool isShowing = false;
    QFont labelFont = QFont();

    QLabel labelDisplay = QLabel();
    QLabel label = QLabel();

public:
    explicit BaseDataFetcher(QObject* parent = nullptr)
        : QObject(parent)
    {
        labelFont.setPointSize(12);
        label.setFont(labelFont);

        // initSettings();
    }

    virtual QString getDisplayName() = 0;

    virtual QString id() = 0;

    virtual QVariant fetchData() = 0;

    virtual void fetchDataAndUpdateLabel() = 0;

    QLabel* getDisplayLabel()
    {
        return &labelDisplay;
    }

    QLabel* getLabel()
    {
        return &label;
    }

    void setShowing(bool showing)
    {
        isShowing = showing;
        if (!inited) {
            inited = true;
            initGlobalDataConnects();
        }
        if (showing) {
            initSettings();
        }
    }

    virtual DisplayInfoSubFunction getType() = 0;

protected:
    void initSettings()
    {
        auto item = globalData->displayInfoSubFunctions()[getType()];
        setLabelDisplay(item);
        setLabelTextAlignment(item);
        setLabelFont(item);
        setLabelTextStyle(item);

        fetchDataAndUpdateLabel();
    }

    void setLabelDisplay(DisplayInfoSubFunctionItem* item)
    {
        if (isShowing && globalData->displayInfoShow()) {
            labelDisplay.setVisible(item->display());
        }
    }
    void setLabelTextAlignment(DisplayInfoSubFunctionItem* item)
    {
        labelDisplay.setAlignment(item->textAlignment());
    }
    void setLabelFont(DisplayInfoSubFunctionItem* item)
    {
        labelDisplay.setFont(QFont(item->fontFamily(), item->textSize()));
    }
    void setLabelTextStyle(DisplayInfoSubFunctionItem* item)
    {
        labelDisplay.setStyleSheet(QString("color: %1;").arg(item->textColor().name()));

        QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
        timerEffect->setColor(item->textShadowColor());
        timerEffect->setBlurRadius(item->textShadowBlurRadius());
        timerEffect->setOffset(item->textShadowOffset());
        labelDisplay.setGraphicsEffect(timerEffect);
    }

    void initGlobalDataConnects()
    {
        auto item = globalData->displayInfoSubFunctions()[getType()];
        connect(item, &DisplayInfoSubFunctionItem::displayChanged, this,
            [this, item](bool newDisplay) {
                setLabelDisplay(item);
            });

        connect(item, &DisplayInfoSubFunctionItem::textAlignmentChanged, this,
            [this, item](const Qt::Alignment& newTextAlignment) {
                setLabelTextAlignment(item);
            });

        connect(item, &DisplayInfoSubFunctionItem::textSizeChanged, this,
            [this, item](int newTextSize) {
                setLabelFont(item);
            });

        connect(item, &DisplayInfoSubFunctionItem::textColorChanged, this,
            [this, item](const QColor& newTextColor) {
                setLabelTextStyle(item);
            });

        connect(item, &DisplayInfoSubFunctionItem::textShadowColorChanged, this,
            [this, item](const QColor& newTextShadowColor) {
                setLabelTextStyle(item);
            });

        connect(item, &DisplayInfoSubFunctionItem::textShadowBlurRadiusChanged, this,
            [this, item](int newTextShadowBlurRadius) {
                setLabelTextStyle(item);
            });

        connect(item, &DisplayInfoSubFunctionItem::textShadowOffsetChanged, this,
            [this, item](QPointF newTextShadowOffset) {
                setLabelTextStyle(item);
            });

        connect(item, &DisplayInfoSubFunctionItem::fontFamilyChanged, this,
            [this, item](const QString& newFontFamily) {
                setLabelFont(item);
            });
    }

    QVariant lastValue;

    void updateLabel(QVariant newValue, QString pattern)
    {
        // 没在显示，不更新
        if (!isShowing) {
            lastValue.clear();
            return;
        }
        // 若跟上次更新的数据一样则直接跳过
        if (!lastValue.isNull() && lastValue == newValue) {
            return;
        }

        lastValue.setValue(newValue);

        auto valueStr = newValue.toString();
        auto valueMetaType = newValue.metaType().id();
        switch (valueMetaType) {
        case QMetaType::Double:
            valueStr = QString::number(newValue.toDouble(), 'f', 2);
            break;
        case QMetaType::Float:
            valueStr = QString::number(newValue.toFloat(), 'f', 2);
            break;
        default:
            break;
        }
        auto text = pattern.arg(valueStr);
        labelDisplay.setText(text);
        label.setText(text);
    }

    template <typename T>
    T getLocalData(int index)
    {
        T buffer;
        memoryUtil->read(memoryUtil->missionPtr + (0x8 * index), &buffer, sizeof(T));
        return buffer;
    }

    template <typename T>
    T getGlobalData(int index)
    {
        long long buffer;
        memoryUtil->read(memoryUtil->globalPtr + (0x8 * (index >> 0x12 & 0x3F)), &buffer, 8);
        T data;
        memoryUtil->read(buffer + (8 * (index & 0x3FFFF)), &data, sizeof(T));
        return data;
    }
};
