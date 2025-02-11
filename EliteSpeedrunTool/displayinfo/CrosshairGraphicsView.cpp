#include "CrosshairGraphicsView.h"

CrosshairGraphicsView::CrosshairGraphicsView(QWidget* parent)
    : QGraphicsView { parent }
{
    setFrameStyle(QFrame::NoFrame);
    setBackgroundBrush(Qt::NoBrush);
    setStyleSheet("background: transparent;");
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
}
