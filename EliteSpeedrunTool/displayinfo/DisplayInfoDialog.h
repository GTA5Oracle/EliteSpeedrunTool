#pragma once

#include "GlobalData.h"
#include <QDialog>
#include <QLabel>

namespace Ui {
class DisplayInfoDialog;
}

class DisplayInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit DisplayInfoDialog(QWidget* parent = nullptr);
    ~DisplayInfoDialog();

    void setDialogBackground(QColor color = globalData->displayInfoBackground());

    void setDisplay();

    void setTextAlignment();

    void setFont();

    void setTextStyle();
    void setTextStyle(
        QLabel* label,
        const QColor& textColor,
        const QColor& textShadowColor,
        qreal textShadowBlurRadius,
        const QPointF& textShadowOffset);

    static const QString timePattern;

    static const QString textQssPattern;

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);
    void removeAllInsertedWidget();
    bool containWidget(QWidget* widget);
    int widgetCount();

    QList<QWidget*> insertedWidget = {};

private:
    void initGlobalDataConnects();

    void setTouchable(bool touchable = globalData->displayInfoTouchable());

    void setChildrenTransparentForMouseEvents(bool transparent = true);

    Ui::DisplayInfoDialog* ui;

    QPointF mousePressedPos;

    QMap<DisplayInfoSubFunction, std::function<void(QVariant)>> subFuncs = {
        qMakePair(DisplayInfoSubFunction::Timer, [this](QVariant v) { return setTime(v); }),
        qMakePair(DisplayInfoSubFunction::Act3Headshot, [this](QVariant v) { return setAct3Headshot(v); })
    };

    void initDisplayData();
    void setTime(QVariant v);
    void setAct3Headshot(QVariant v);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
};
