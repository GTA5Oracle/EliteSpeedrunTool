#include "HotkeyRedirector.h"
#include <windows.h>

HotkeyRedirector::HotkeyRedirector(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
}

bool HotkeyRedirector::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    MSG* msg = static_cast<MSG*>(message);
    switch (msg->message) {
    case WM_PARENTNOTIFY:
        if (LOWORD(msg->wParam) == WM_DESTROY) {
            SetParent(reinterpret_cast<HWND>(winId()), nullptr);
        }
        break;
    }
    return QWidget::nativeEvent(eventType, message, result);
}
