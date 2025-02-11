#pragma once

#include "ui_CrosshairDialog.h"
#include <QDialog>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
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

    DWORD pid = 0;
    RECT windowRect = { 0, 0, 0, 0 };
    QTimer timer;

    void closeEvent(QCloseEvent* event);
    void init();
    void initWindowProp();
    void hookProcessEvent();
    void unhookProcessEvent();
    bool scaledClientRect(RECT& rect);
    void setWindowSize();
    void setCrosshairOffset();
    void setCrosshairSize();
    void setCrosshairImage();
    void setCrosshairShadow();
    void setCrosshairColor();
    void setCrosshairOpacity();

    void loadImage(QString path);

private:
    Ui::CrosshairDialog ui;
    QGraphicsScene scene;
    HWINEVENTHOOK locationChangeEventHook;
    QGraphicsDropShadowEffect* imageShadow = new QGraphicsDropShadowEffect(this);
};
