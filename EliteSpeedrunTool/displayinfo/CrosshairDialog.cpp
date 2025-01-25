#include "CrosshairDialog.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include <QMovie>
#include <QPainter>
#include <QScreen>
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
    unhookGtaProcessEvent();
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
        HWND gtaHwnd = memoryUtil->getWindowHwnd();
        switch (event) {
        case EVENT_OBJECT_LOCATIONCHANGE:
            if (gtaHwnd == hwnd) {
                RECT rect;
                if (inst->scaledClientRect(rect)) {
                    inst->gtaRect = rect;
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
    ui.labImage->setAttribute(Qt::WA_TransparentForMouseEvents);

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    initGtaWindowProp();

    setCrosshairShadow();

    connect(globalData, &GlobalData::crosshairOffsetChanged, this, &CrosshairDialog::setCrosshairOffset);
    connect(globalData, &GlobalData::crosshairSizeChanged, this, &CrosshairDialog::setCrosshairSize);
    connect(globalData, &GlobalData::crosshairImageChanged, this, &CrosshairDialog::setCrosshairImage);
    connect(globalData, &GlobalData::crosshairShadowBlurRadiusChanged, this, &CrosshairDialog::setCrosshairShadow);
    connect(globalData, &GlobalData::crosshairShadowColorChanged, this, &CrosshairDialog::setCrosshairShadow);
    connect(globalData, &GlobalData::crosshairShadowOffsetChanged, this, &CrosshairDialog::setCrosshairShadow);

    connect(&timer, &QTimer::timeout, this, [this]() { initGtaWindowProp(); });
    timer.start(1000);
}

void CrosshairDialog::initGtaWindowProp()
{
    DWORD currentGtaPid = memoryUtil->getPid();
    if (gtaPid != currentGtaPid) {
        gtaPid = currentGtaPid;

        unhookGtaProcessEvent();

        if (currentGtaPid == 0) {
            setVisible(false);
            return;
        }

        hookGtaProcessEvent();

        RECT rect;
        if (scaledClientRect(rect)) {
            gtaRect = rect;
            setWindowSize();
            setCrosshairSize();
        }
    }

    QTimer::singleShot(0, this, [this]() { setVisible(memoryUtil->getWindowHwnd() == GetForegroundWindow()); });
}

void CrosshairDialog::hookGtaProcessEvent()
{
    locationChangeEventHook = SetWinEventHook(
        EVENT_OBJECT_LOCATIONCHANGE,
        EVENT_OBJECT_LOCATIONCHANGE,
        nullptr, winEventProc, gtaPid, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}

void CrosshairDialog::unhookGtaProcessEvent()
{
    UnhookWinEvent(locationChangeEventHook);
}

bool CrosshairDialog::scaledClientRect(RECT& rect)
{
    HWND hwnd = memoryUtil->getWindowHwnd();
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
        if (r.top != gtaRect.top || r.bottom != gtaRect.bottom || r.left != gtaRect.left || r.right != gtaRect.right) {
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
    setGeometry(gtaRect.left, gtaRect.top, gtaRect.right - gtaRect.left, gtaRect.bottom - gtaRect.top);
    setCrosshairOffset();
}

void CrosshairDialog::setCrosshairOffset()
{
    auto crosshairSize = globalData->crosshairSize();
    ui.labImage->move(QPoint(width() / 2, height() / 2) + globalData->crosshairOffset() - QPoint(crosshairSize.width(), crosshairSize.height()) / 2);
}

void CrosshairDialog::setCrosshairSize()
{
    auto size = globalData->crosshairSize();
    auto labPos = ui.labImage->pos();
    ui.labImage->setGeometry(labPos.x(), labPos.y(), size.width(), size.height());
    setCrosshairImage();
}

void CrosshairDialog::setCrosshairImage()
{
    auto image = globalData->crosshairImage();
    if (image.isEmpty()) {
        QPixmap p(ui.labImage->size());
        loadSvgImage(":/image/ic_crosshair.svg", p);
        ui.labImage->setPixmap(p);
    } else {
        if (image.endsWith(".svg", Qt::CaseInsensitive)) {
            QPixmap p(ui.labImage->size());
            loadSvgImage(image, p);
            ui.labImage->setPixmap(p);
        } else if (image.endsWith(".gif", Qt::CaseInsensitive)) {
            QMovie* movie = new QMovie(image);
            ui.labImage->setMovie(movie);
            movie->start();
        } else {
            ui.labImage->setPixmap(QPixmap(image));
        }
    }
}

void CrosshairDialog::setCrosshairShadow()
{
    imageShadow->setBlurRadius(globalData->crosshairShadowBlurRadius());
    imageShadow->setColor(globalData->crosshairShadowColor());
    imageShadow->setOffset(globalData->crosshairShadowOffset());
    ui.labImage->setGraphicsEffect(imageShadow);
}

void CrosshairDialog::loadSvgImage(QString path, QPixmap& pixmap)
{
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QSvgRenderer svg(path);
    svg.render(&painter);
}
