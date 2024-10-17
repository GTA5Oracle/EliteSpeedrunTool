#include "RegionSelectorGraphicsView.h"
#include <QMouseEvent>

RegionSelectorGraphicsView::RegionSelectorGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    setMouseTracking(true);
}

void RegionSelectorGraphicsView::setIgnoreMouseEvent(bool ignore)
{
    ignoreMouseEvent = ignore;
}

void RegionSelectorGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (ignoreMouseEvent)
        event->ignore();
}

void RegionSelectorGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (ignoreMouseEvent)
        event->ignore();
}

void RegionSelectorGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (ignoreMouseEvent)
        event->ignore();
}
