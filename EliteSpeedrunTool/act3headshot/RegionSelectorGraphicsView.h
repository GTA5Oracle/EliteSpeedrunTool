#pragma once

#include <QGraphicsView>

class RegionSelectorGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    RegionSelectorGraphicsView(QWidget* parent = nullptr);

    void setIgnoreMouseEvent(bool ignore);

private:
    bool ignoreMouseEvent;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};
