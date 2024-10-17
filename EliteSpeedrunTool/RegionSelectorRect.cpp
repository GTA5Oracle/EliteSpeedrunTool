#include "RegionSelectorRect.h"
#include <QMouseEvent>

RegionSelectorRect::RegionSelectorRect(QWidget* parent, Qt::WindowFlags)
    : QFrame(parent)
{
    setMouseTracking(true);
}

void RegionSelectorRect::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
    emit mouseMove(event->pos());
}
