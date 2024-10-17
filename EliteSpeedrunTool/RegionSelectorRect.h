#ifndef REGIONSELECTORRECT_H
#define REGIONSELECTORRECT_H

#include <QFrame>

class RegionSelectorRect : public QFrame {
    Q_OBJECT

signals:
    void mouseMove(QPoint p);

public:
    RegionSelectorRect(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    void mouseMoveEvent(QMouseEvent* event);
};

#endif // REGIONSELECTORRECT_H
