#pragma once

#include "ui_HotkeyRedirector.h"
#include <QWidget>

class HotkeyRedirector : public QWidget {
    Q_OBJECT

public:
    explicit HotkeyRedirector(QWidget* parent = nullptr);

private:
    Ui::HotkeyRedirector ui;

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
};
