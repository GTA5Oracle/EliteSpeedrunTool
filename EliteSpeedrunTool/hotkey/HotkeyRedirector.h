#pragma once

#include <QWidget>

namespace Ui {
class HotkeyRedirector;
}

class HotkeyRedirector : public QWidget {
    Q_OBJECT

public:
    explicit HotkeyRedirector(QWidget* parent = nullptr);
    ~HotkeyRedirector();

private:
    Ui::HotkeyRedirector* ui;

protected:
    void closeEvent(QCloseEvent* event);
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
};
