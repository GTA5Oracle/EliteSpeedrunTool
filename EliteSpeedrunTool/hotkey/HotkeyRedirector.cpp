#include "HotkeyRedirector.h"

HotkeyRedirector::HotkeyRedirector(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
}

bool HotkeyRedirector::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    // MSG* msg = static_cast<MSG*>(message);
    // qInfo() << "HotkeyRedirector nativeEvent: message: " << msg->message << " wParam: " << msg->wParam;
    return QWidget::nativeEvent(eventType, message, result);
}
