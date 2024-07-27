#pragma once

#include "GlobalData.h"
#include <QDialog>

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
    void setTimerTextStyle(
        const QColor& textColor,
        const QColor& textShadowColor,
        qreal textShadowBlurRadius,
        const QPointF& textShadowOffset);
    void setAutoTimerTextStyle(
        const QColor& textColor,
        const QColor& textShadowColor,
        qreal textShadowBlurRadius,
        const QPointF& textShadowOffset);

    void setTime(unsigned int m, unsigned int s, unsigned int ms);
    void setAutoTime(unsigned int m, unsigned int s, unsigned int ms);

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

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
};
