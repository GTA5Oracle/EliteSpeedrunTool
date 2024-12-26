#include "HotkeyRedirector.h"
#include "MemoryUtil.h"
#include "ui_HotkeyRedirector.h"

HotkeyRedirector::HotkeyRedirector(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::HotkeyRedirector)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
}

HotkeyRedirector::~HotkeyRedirector()
{
    delete ui;
}

void HotkeyRedirector::closeEvent(QCloseEvent* event)
{
    SetForegroundWindow(memoryUtil->getWindowHwnd());
    QWidget::closeEvent(event);
}

bool HotkeyRedirector::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    MSG* msg = static_cast<MSG*>(message);
    // qInfo() << "HotkeyRedirector nativeEvent: message: " << msg->message << " wParam: " << msg->wParam;
    return QWidget::nativeEvent(eventType, message, result);
}
