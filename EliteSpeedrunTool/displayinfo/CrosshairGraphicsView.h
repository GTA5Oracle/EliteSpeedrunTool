#pragma once

#include <QGraphicsView>

class CrosshairGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    CrosshairGraphicsView(QWidget* parent = nullptr);
};
