#include "CursorSelectorButton.h"

#include <QMouseEvent>

CursorSelectorButton::CursorSelectorButton(QWidget* parent)
    : QToolButton { parent }
{
}

void CursorSelectorButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        mousePressed = true;
    }
    QToolButton::mousePressEvent(event);
}

void CursorSelectorButton::mouseReleaseEvent(QMouseEvent* event)
{
    if (mousePressed) {
        emit realReleased();
    }
    mousePressed = false;
    QToolButton::mouseReleaseEvent(event);
}
