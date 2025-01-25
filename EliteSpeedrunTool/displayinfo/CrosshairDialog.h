#pragma once

#include "ui_CrosshairDialog.h"
#include <QDialog>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <windows.h>

class CrosshairDialog : public QDialog {
    Q_OBJECT

public:
    static void showCrosshair();
    static void closeCrosshair();

    static void CALLBACK winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
        LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

protected:
    explicit CrosshairDialog(QWidget* parent = nullptr);
    static CrosshairDialog* inst;

    DWORD gtaPid = 0;
    RECT gtaRect = { 0, 0, 0, 0 };
    QTimer timer;

    void closeEvent(QCloseEvent* event);
    void init();
    void initGtaWindowProp();
    void hookGtaProcessEvent();
    void unhookGtaProcessEvent();
    bool scaledClientRect(RECT& rect);
    void setWindowSize();
    void setCrosshairOffset();
    void setCrosshairSize();
    void setCrosshairImage();
    void setCrosshairShadow();

    void loadSvgImage(QString path, QPixmap& pixmap);

private:
    Ui::CrosshairDialog ui;
    HWINEVENTHOOK locationChangeEventHook;
    QGraphicsDropShadowEffect* imageShadow = new QGraphicsDropShadowEffect(this);
};
