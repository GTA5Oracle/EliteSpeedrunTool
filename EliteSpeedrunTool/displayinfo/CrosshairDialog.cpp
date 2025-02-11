#include "CrosshairDialog.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include <QDebug>
#include <QGraphicsSvgItem>
#include <QMovie>
#include <QPainter>
#include <QScreen>
#include <QScrollBar>
#include <QSvgRenderer>

CrosshairDialog* CrosshairDialog::inst = nullptr;

CrosshairDialog::CrosshairDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    init();
}

void CrosshairDialog::closeEvent(QCloseEvent* event)
{
    timer.stop();
    inst = nullptr;
    unhookProcessEvent();
}

void CrosshairDialog::showCrosshair()
{
    if (!inst) {
        inst = new CrosshairDialog;
        connect(globalData, &GlobalData::crosshairShowChanged, inst, []() {
            if (!globalData->crosshairShow()) {
                CrosshairDialog::closeCrosshair();
            }
        });
    }
    inst->show();
}

void CrosshairDialog::closeCrosshair()
{
    if (inst) {
        inst->close();
        inst = nullptr;
    }
}

void CALLBACK CrosshairDialog::winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
    LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    if (CrosshairDialog::inst) {
        HWND attachWindowHwnd = memoryUtil->getWindowHwnd(globalData->crosshairAttachWindowTitle());
        switch (event) {
        case EVENT_OBJECT_LOCATIONCHANGE:
            if (attachWindowHwnd == hwnd) {
                RECT rect;
                if (inst->scaledClientRect(rect)) {
                    inst->windowRect = rect;
                    inst->setWindowSize();
                }
            }
            break;
        }
    }
}

void CrosshairDialog::init()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    ui.graphicsView->setScene(&scene);

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    setCrosshairImage();
    initWindowProp();

    setCrosshairShadow();

    connect(globalData, &GlobalData::crosshairOffsetChanged, this, &CrosshairDialog::setCrosshairOffset);
    connect(globalData, &GlobalData::crosshairSizeChanged, this, &CrosshairDialog::setCrosshairSize);
    connect(globalData, &GlobalData::crosshairImageChanged, this, &CrosshairDialog::setCrosshairImage);
    connect(globalData, &GlobalData::crosshairColorChanged, this, &CrosshairDialog::setCrosshairColor);
    connect(globalData, &GlobalData::crosshairOpacityChanged, this, &CrosshairDialog::setCrosshairOpacity);
    connect(globalData, &GlobalData::crosshairShadowBlurRadiusChanged, this, &CrosshairDialog::setCrosshairShadow);
    connect(globalData, &GlobalData::crosshairShadowColorChanged, this, &CrosshairDialog::setCrosshairShadow);
    connect(globalData, &GlobalData::crosshairShadowOffsetChanged, this, &CrosshairDialog::setCrosshairShadow);

    connect(&timer, &QTimer::timeout, this, [this]() { initWindowProp(); });
    timer.start(1000);
}

void CrosshairDialog::initWindowProp()
{
    DWORD currentPid = memoryUtil->getPid(memoryUtil->getWindowHwnd(globalData->crosshairAttachWindowTitle()));
    if (pid != currentPid) {
        pid = currentPid;

        unhookProcessEvent();

        if (currentPid == 0) {
            setVisible(false);
            return;
        }

        hookProcessEvent();

        RECT currentRect;
        if (scaledClientRect(currentRect)) {
            windowRect = currentRect;
            setWindowSize();
            setCrosshairSize();
        }
    }

    QTimer::singleShot(0, this, [this]() {
        setVisible(memoryUtil->getWindowHwnd(globalData->crosshairAttachWindowTitle()) == GetForegroundWindow());
    });
}

void CrosshairDialog::hookProcessEvent()
{
    locationChangeEventHook = SetWinEventHook(
        EVENT_OBJECT_LOCATIONCHANGE,
        EVENT_OBJECT_LOCATIONCHANGE,
        nullptr, winEventProc, pid, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}

void CrosshairDialog::unhookProcessEvent()
{
    UnhookWinEvent(locationChangeEventHook);
}

bool CrosshairDialog::scaledClientRect(RECT& rect)
{
    HWND hwnd = memoryUtil->getWindowHwnd(globalData->crosshairAttachWindowTitle());
    if (!hwnd) {
        hwnd = GetDesktopWindow();
    }
    RECT r;
    if (GetClientRect(hwnd, &r)) {
        MapWindowPoints(hwnd, nullptr, (LPPOINT)&r, 2);
        qreal devicePixelRatio = screen()->devicePixelRatio();
        r.left /= devicePixelRatio;
        r.top /= devicePixelRatio;
        r.right /= devicePixelRatio;
        r.bottom /= devicePixelRatio;
        if (r.top != windowRect.top || r.bottom != windowRect.bottom || r.left != windowRect.left || r.right != windowRect.right) {
            rect.left = r.left;
            rect.top = r.top;
            rect.right = r.right;
            rect.bottom = r.bottom;
            return true;
        }
    }
    return false;
}

void CrosshairDialog::setWindowSize()
{
    setGeometry(windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
    ui.graphicsView->setGeometry(0, 0, width(), height());
    scene.setSceneRect(ui.graphicsView->frameRect());
    setCrosshairOffset();
}

void CrosshairDialog::setCrosshairOffset()
{
    auto sceneSize = scene.sceneRect().center();
    for (auto item : scene.items()) {
        auto sceneBoundingRect = item->sceneBoundingRect();
        item->setPos(sceneSize + globalData->crosshairOffset() - QPointF(sceneBoundingRect.width(), sceneBoundingRect.height()) / 2);
    }
}

void CrosshairDialog::setCrosshairSize()
{
    auto crosshairSize = globalData->crosshairSize().toSizeF();
    for (auto item : scene.items()) {
        auto itemWidth = item->boundingRect().width();
        auto itemHeight = item->boundingRect().height();
        item->setTransform(QTransform::fromScale(crosshairSize.width() / itemWidth, crosshairSize.height() / itemHeight));
        ui.graphicsView->update(); // Fixed the item size jumping bug
    }
    setCrosshairOffset();
}

void CrosshairDialog::setCrosshairImage()
{
    auto image = globalData->crosshairImage();
    if (image.isEmpty()) {
        loadImage(":/image/ic_crosshair_1.png");
    } else {
        loadImage(image);
    }
    setCrosshairSize();
    setCrosshairColor();
    setCrosshairOpacity();
    setCrosshairOffset();
}

void CrosshairDialog::setCrosshairShadow()
{
    imageShadow->setBlurRadius(globalData->crosshairShadowBlurRadius());
    imageShadow->setColor(globalData->crosshairShadowColor());
    imageShadow->setOffset(globalData->crosshairShadowOffset());
    ui.graphicsView->setGraphicsEffect(imageShadow);
}

void CrosshairDialog::setCrosshairColor()
{
    auto itemColor = globalData->crosshairColor();
    if (!itemColor.isValid()) {
        for (auto item : scene.items()) {
            item->setGraphicsEffect(nullptr);
        }
        return;
    }
    for (auto item : scene.items()) {
        QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
        effect->setColor(itemColor);
        item->setGraphicsEffect(effect);
    }
}

void CrosshairDialog::setCrosshairOpacity()
{
    for (auto item : scene.items()) {
        item->setOpacity(globalData->crosshairOpacity());
    }
}

void CrosshairDialog::loadImage(QString path)
{
    QGraphicsItem* item = nullptr;
    if (path.endsWith(".svg", Qt::CaseInsensitive)) {
        item = new QGraphicsSvgItem(path);
    } else if (path.endsWith(".gif", Qt::CaseInsensitive)) {
    } else {
        QPixmap p(path);
        item = new QGraphicsPixmapItem(p);
    }
    if (item) {
        scene.clear();
        scene.addItem(item);
    }
}
