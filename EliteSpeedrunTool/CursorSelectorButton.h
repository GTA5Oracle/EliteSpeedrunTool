#pragma once

#include <QToolButton>

class CursorSelectorButton : public QToolButton {
    Q_OBJECT
public:
    CursorSelectorButton(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool mousePressed = false;

signals:
    void realReleased();
};
