#ifndef REGIONSELECTORDIALOG_H
#define REGIONSELECTORDIALOG_H

#include <QDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>

namespace Ui {
class RegionSelectorDialog;
}

class RegionSelectorDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegionSelectorDialog(QRect rect, QWidget* parent = nullptr);
    ~RegionSelectorDialog();

    QRect getRect();

private:
    Ui::RegionSelectorDialog* ui;

    QRect region;

    QColor grayColor = QColor(0, 0, 0, 180);

    QGraphicsItem* grayItem = nullptr;

    float previewZoomRate = 3.2f;

    QGraphicsScene* scene = new QGraphicsScene(this);

    QPixmap pixmap;
    QImage image;

    QColor pixelColor = QColor();

    QPixmap* resultPixmap = nullptr;

    int rectLineWidth = 1;
    int pointRadius = 9;

    int deltaHeight = 0;
    int deltaWidth = 0;

    bool hasRect = false;
    bool mousePressed = false;
    QPointF pressedPos;

    QPointF lastPos;

    bool pressedInRectArea = false;
    bool pressedInLeftTopArea = false;
    bool pressedInRightTopArea = false;
    bool pressedInLeftBottomArea = false;
    bool pressedInRightBottomArea = false;
    bool pressedInLeftArea = false;
    bool pressedInRightArea = false;
    bool pressedInTopArea = false;
    bool pressedInBottomArea = false;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

    void refreshPointPos(); // 更新各个点位置
    QRect checkRectVaild(QRect r); // 调整Rect的位置确保在屏幕内
    void setRectVisible(bool visible);

    void refreshGrayArea(); // 更新灰色区域
    void refreshPreviewArea(QPointF mousePos); // 更新预览区域
};

#endif // REGIONSELECTORDIALOG_H
