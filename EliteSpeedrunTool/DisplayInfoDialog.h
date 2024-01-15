#ifndef SCREENSHOTDISPLAYDIALOG_H
#define SCREENSHOTDISPLAYDIALOG_H

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

    void setTime(int m, int s, int ms);
    void setAutoTime(int m, int s, int ms);

    static const QString timePattern;

    static const QString textQssPattern;

    void insertWidget(int index, QWidget* widget);
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

#endif // SCREENSHOTDISPLAYDIALOG_H
