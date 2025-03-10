#include "RegionSelectorDialog.h"
#include "MemoryUtil.h"
#include "ui_RegionSelectorDialog.h"
#include <QMouseEvent>
#include <QPainter>
#include <windows.h>

RegionSelectorDialog::RegionSelectorDialog(QRect rect, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::RegionSelectorDialog)
{
    ui->setupUi(this);

    auto devicePixelRatio = devicePixelRatioF();
    // 多显示器支持
    move(GetSystemMetrics(SM_XVIRTUALSCREEN) / devicePixelRatio,
        GetSystemMetrics(SM_YVIRTUALSCREEN) / devicePixelRatio);
    resize(GetSystemMetrics(SM_CXVIRTUALSCREEN) / devicePixelRatio,
        GetSystemMetrics(SM_CYVIRTUALSCREEN) / devicePixelRatio);

    QScreen* screen = QGuiApplication::primaryScreen();
    pixmap = screen->grabWindow(0);
    pixmap.setDevicePixelRatio(devicePixelRatio);
    image = pixmap.toImage();

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen | Qt::WindowActive);

    HWND windowHwnd = memoryUtil->getGtaWindowHwnd();
    if (windowHwnd) {
        ShowWindow(windowHwnd, SW_SHOWMINIMIZED);
    }

    ui->widgetInfoContainer->adjustSize();
    ui->widgetInfoContainer->setVisible(false);
    ui->widgetInfoContainer->setAttribute(Qt::WA_TransparentForMouseEvents, true); // 信息Widget不接收鼠标事件

    // 可在不点击鼠标的情况下捕获移动事件
    setMouseTracking(true);
    setTabletTracking(true);
    scene->setSceneRect(QRectF(0, 0, width(), height()));
    scene->addPixmap(pixmap);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setGeometry(0, 0, width(), height());
    ui->graphicsView->setIgnoreMouseEvent(true);

    QString frameRectStyle = QString("border: ") + QString::number(rectLineWidth) + QString(" solid rgb(255, 255, 0)");
    ui->frameRect->setStyleSheet(frameRectStyle);
    // 初始化8个点的鼠标样式
    ui->frameRect->setCursor(Qt::SizeAllCursor);
    ui->widgetLeftTopPoint->setCursor(Qt::SizeFDiagCursor);
    ui->widgetRightBottomPoint->setCursor(Qt::SizeFDiagCursor);
    ui->widgetLeftBottomPoint->setCursor(Qt::SizeBDiagCursor);
    ui->widgetRightTopPoint->setCursor(Qt::SizeBDiagCursor);
    ui->widgetLeftPoint->setCursor(Qt::SizeHorCursor);
    ui->widgetRightPoint->setCursor(Qt::SizeHorCursor);
    ui->widgetTopPoint->setCursor(Qt::SizeVerCursor);
    ui->widgetBottomPoint->setCursor(Qt::SizeVerCursor);
    QString pointStyle = "background: rgb(255, 255, 0)";
    // 初始化8个点的外观样式
    ui->widgetLeftTopPoint->setStyleSheet(pointStyle);
    ui->widgetRightBottomPoint->setStyleSheet(pointStyle);
    ui->widgetLeftBottomPoint->setStyleSheet(pointStyle);
    ui->widgetRightTopPoint->setStyleSheet(pointStyle);
    ui->widgetLeftPoint->setStyleSheet(pointStyle);
    ui->widgetRightPoint->setStyleSheet(pointStyle);
    ui->widgetTopPoint->setStyleSheet(pointStyle);
    ui->widgetBottomPoint->setStyleSheet(pointStyle);

    // 接收矩形区域内鼠标移动的位置
    connect(ui->frameRect, &RegionSelectorRect::mouseMove, this, [=](QPoint p) {
        refreshPreviewArea(p);
    });

    ui->frameRect->setGeometry(0, 0, 0, 0);
    setRectVisible(false);
    refreshPointPos();
    refreshGrayArea();
    update();
}

RegionSelectorDialog::~RegionSelectorDialog()
{
    scene->removeItem(grayItem);
    delete grayItem;
    resultPixmap = nullptr;
    delete ui;
}

QRect RegionSelectorDialog::getRect()
{
    return region;
}

void RegionSelectorDialog::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    mousePressed = true;
    pressedPos = event->pos();
    lastPos = event->pos();

    // 判断在哪个区域按下的鼠标
    if (ui->widgetLeftTopPoint->geometry().contains(event->pos()))
        pressedInLeftTopArea = true;
    else if (ui->widgetLeftBottomPoint->geometry().contains(event->pos()))
        pressedInLeftBottomArea = true;
    else if (ui->widgetRightTopPoint->geometry().contains(event->pos()))
        pressedInRightTopArea = true;
    else if (ui->widgetRightBottomPoint->geometry().contains(event->pos()))
        pressedInRightBottomArea = true;
    else if (ui->widgetLeftPoint->geometry().contains(event->pos()))
        pressedInLeftArea = true;
    else if (ui->widgetRightPoint->geometry().contains(event->pos()))
        pressedInRightArea = true;
    else if (ui->widgetTopPoint->geometry().contains(event->pos()))
        pressedInTopArea = true;
    else if (ui->widgetBottomPoint->geometry().contains(event->pos()))
        pressedInBottomArea = true;
    else if (ui->frameRect->geometry().contains(event->pos()))
        pressedInRectArea = true;
}

void RegionSelectorDialog::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    if (!hasRect) {
        hasRect = true;
    }

    mousePressed = false;
    pressedInRectArea = false;
    pressedInLeftTopArea = false;
    pressedInLeftBottomArea = false;
    pressedInRightTopArea = false;
    pressedInRightBottomArea = false;
    pressedInLeftArea = false;
    pressedInTopArea = false;
    pressedInRightArea = false;
    pressedInBottomArea = false;

    deltaHeight = deltaWidth = 0;
}

void RegionSelectorDialog::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
    if (mousePressed) {
        if (!hasRect) {
            // 首次绘制矩形框
            QPointF pos = event->pos();
            QRect r = QRect(std::min(pos.x(), pressedPos.x()),
                std::min(pos.y(), pressedPos.y()),
                std::abs(pos.x() - pressedPos.x()),
                std::abs(pos.y() - pressedPos.y()));
            setRectVisible(true);
            ui->frameRect->setGeometry(r);
            refreshPointPos();
        } else {
            QRect frameRectGeo = ui->frameRect->geometry();
            // 左上点
            if (pressedInLeftTopArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x() - lastPos.x() + event->position().x() + deltaWidth,
                    frameRectGeo.y() - lastPos.y() + event->position().y() + deltaHeight,
                    frameRectGeo.width() + lastPos.x() - event->position().x() - deltaWidth,
                    frameRectGeo.height() + lastPos.y() - event->position().y() - deltaHeight));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.height() < 0) {
                    deltaHeight = r.height();
                    pressedInLeftTopArea = false;
                    pressedInLeftBottomArea = true;
                    r.setHeight(0);
                }
                if (r.width() < 0) {
                    deltaWidth = r.width();
                    pressedInLeftTopArea = false;
                    pressedInRightTopArea = true;
                    r.setWidth(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
                // 左下点
            } else if (pressedInLeftBottomArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x() - lastPos.x() + event->position().x() + deltaWidth,
                    frameRectGeo.y() + deltaHeight,
                    frameRectGeo.width() + lastPos.x() - event->position().x() - deltaWidth,
                    frameRectGeo.height() - lastPos.y() + event->position().y() - deltaHeight));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.height() < 0) {
                    deltaHeight = r.height();
                    pressedInLeftBottomArea = false;
                    pressedInLeftTopArea = true;
                    r.setHeight(0);
                }
                if (r.width() < 0) {
                    deltaWidth = r.width();
                    pressedInLeftBottomArea = false;
                    pressedInRightBottomArea = true;
                    r.setWidth(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
                // 右上点
            } else if (pressedInRightTopArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x() + deltaWidth,
                    frameRectGeo.y() - lastPos.y() + event->position().y() + deltaHeight,
                    frameRectGeo.width() - lastPos.x() + event->position().x() - deltaWidth,
                    frameRectGeo.height() + lastPos.y() - event->position().y() - deltaHeight));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.height() < 0) {
                    deltaHeight = r.height();
                    pressedInRightTopArea = false;
                    pressedInRightBottomArea = true;
                    r.setHeight(0);
                }
                if (r.width() < 0) {
                    deltaWidth = r.width();
                    pressedInRightTopArea = false;
                    pressedInLeftTopArea = true;
                    r.setWidth(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
                // 右下点
            } else if (pressedInRightBottomArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x() + deltaWidth,
                    frameRectGeo.y() + deltaHeight,
                    frameRectGeo.width() - lastPos.x() + event->position().x() - deltaWidth,
                    frameRectGeo.height() - lastPos.y() + event->position().y() - deltaHeight));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.height() < 0) {
                    deltaHeight = r.height();
                    pressedInRightBottomArea = false;
                    pressedInRightTopArea = true;
                    r.setHeight(0);
                }
                if (r.width() < 0) {
                    deltaWidth = r.width();
                    pressedInRightBottomArea = false;
                    pressedInLeftBottomArea = true;
                    r.setWidth(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
                // 上中点
            } else if (pressedInTopArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x(),
                    frameRectGeo.y() - lastPos.y() + event->position().y() + deltaHeight,
                    frameRectGeo.width(),
                    frameRectGeo.height() + lastPos.y() - event->position().y() - deltaHeight));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.height() < 0) {
                    deltaHeight = r.height();
                    pressedInTopArea = false;
                    pressedInBottomArea = true;
                    r.setHeight(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
                // 下中点
            } else if (pressedInBottomArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x(),
                    frameRectGeo.y() + deltaHeight,
                    frameRectGeo.width(),
                    frameRectGeo.height() - lastPos.y() + event->position().y() - deltaHeight));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.height() < 0) {
                    deltaHeight = r.height();
                    pressedInBottomArea = false;
                    pressedInTopArea = true;
                    r.setHeight(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
                // 左中点
            } else if (pressedInLeftArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x() - lastPos.x() + event->position().x() + deltaWidth,
                    frameRectGeo.y(),
                    frameRectGeo.width() + lastPos.x() - event->position().x() - deltaWidth,
                    frameRectGeo.height()));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.width() < 0) {
                    deltaWidth = r.width();
                    pressedInLeftArea = false;
                    pressedInRightArea = true;
                    r.setWidth(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
                // 右中点
            } else if (pressedInRightArea) {
                QRect r = checkRectVaild(QRect(frameRectGeo.x() + deltaWidth,
                    frameRectGeo.y(),
                    frameRectGeo.width() - lastPos.x() + event->position().x() - deltaWidth,
                    frameRectGeo.height()));
                deltaHeight = deltaWidth = 0;
                // 水平或竖直缩成一条线后自动切换到相对应的另一个点
                if (r.width() < 0) {
                    deltaWidth = r.width();
                    pressedInRightArea = false;
                    pressedInLeftArea = true;
                    r.setWidth(0);
                }
                ui->frameRect->setGeometry(r);
                refreshPointPos();
            } else if (pressedInRectArea) {
                int newX = frameRectGeo.x() - lastPos.x() + event->position().x();
                int newY = frameRectGeo.y() - lastPos.y() + event->position().y();
                ui->frameRect->move(qMin(rect().right() - frameRectGeo.width() + 1, qMax(newX, rect().left())),
                    qMin(rect().bottom() - frameRectGeo.height() + 1, qMax(newY, rect().top())));
                refreshPointPos();
            }
        }
    }

    // 更新预览放大区域内容
    refreshPreviewArea(event->position());

    lastPos = event->position();
}

// 更新8个点的位置
void RegionSelectorDialog::refreshPointPos()
{
    QRectF r = ui->frameRect->geometry();
    ui->widgetLeftTopPoint->setGeometry(r.x() - pointRadius / 2,
        r.y() - pointRadius / 2,
        pointRadius,
        pointRadius);
    ui->widgetLeftBottomPoint->setGeometry(r.x() - pointRadius / 2,
        r.y() + r.height() - pointRadius / 2,
        pointRadius,
        pointRadius);
    ui->widgetRightTopPoint->setGeometry(r.x() + r.width() - pointRadius / 2,
        r.y() - pointRadius / 2,
        pointRadius,
        pointRadius);
    ui->widgetRightBottomPoint->setGeometry(r.x() + r.width() - pointRadius / 2,
        r.y() + r.height() - pointRadius / 2,
        pointRadius,
        pointRadius);

    ui->widgetLeftPoint->setGeometry(r.x() - pointRadius / 2,
        (2 * r.y() + r.height()) / 2 - pointRadius / 2,
        pointRadius,
        pointRadius);
    ui->widgetRightPoint->setGeometry(r.x() + r.width() - pointRadius / 2,
        (2 * r.y() + r.height()) / 2 - pointRadius / 2,
        pointRadius,
        pointRadius);
    ui->widgetTopPoint->setGeometry((2 * r.x() + r.width()) / 2 - pointRadius / 2,
        r.y() - pointRadius / 2,
        pointRadius,
        pointRadius);
    ui->widgetBottomPoint->setGeometry((2 * r.x() + r.width()) / 2 - pointRadius / 2,
        r.y() + r.height() - pointRadius / 2,
        pointRadius,
        pointRadius);

    refreshGrayArea();
}

// 调整Rect的位置确保在屏幕内
QRect RegionSelectorDialog::checkRectVaild(QRect r)
{
    QRect newRect = QRect(qMax(0, r.x()),
        qMax(0, r.y()),
        qMin(width() - r.x(), r.width()),
        qMin(height() - r.y(), r.height()));
    newRect.setWidth(newRect.width() - (newRect.x() - r.x()));
    newRect.setHeight(newRect.height() - (newRect.y() - r.y()));
    return newRect;
}

// 设置选框和8个点的可见性
void RegionSelectorDialog::setRectVisible(bool visible)
{
    ui->frameRect->setVisible(visible);
    ui->widgetLeftTopPoint->setVisible(visible);
    ui->widgetLeftBottomPoint->setVisible(visible);
    ui->widgetRightTopPoint->setVisible(visible);
    ui->widgetRightBottomPoint->setVisible(visible);
    ui->widgetLeftPoint->setVisible(visible);
    ui->widgetRightPoint->setVisible(visible);
    ui->widgetTopPoint->setVisible(visible);
    ui->widgetBottomPoint->setVisible(visible);
}

// 绘制灰色阴影
void RegionSelectorDialog::refreshGrayArea()
{
    if (grayItem) {
        scene->removeItem(grayItem);
        delete grayItem;
    }
    QPainterPath grayPath = QPainterPath();
    grayPath.addRegion(QRegion(geometry()).subtracted(ui->frameRect->geometry()));
    grayItem = scene->addPolygon(grayPath.toFillPolygon(),
        QPen(QColorConstants::Transparent),
        QBrush(grayColor));
}

// 更新预览放大内容
void RegionSelectorDialog::refreshPreviewArea(QPointF mousePos)
{
    auto devicePixelRatio = ui->graphicsView->devicePixelRatio();
    QPoint devicePixelRatioMousePos = QPoint(
        round(mousePos.x() * devicePixelRatio),
        round(mousePos.y() * devicePixelRatio));
    // 更新预判Widget位置---------------------------
    CURSORINFO ci;
    ICONINFO iconInf;
    ci.cbSize = sizeof(CURSORINFO);
    GetCursorInfo(&ci);
    GetIconInfo(ci.hCursor, &iconInf);
    int infoContainerX = mousePos.x() + GetSystemMetrics(SM_CXCURSOR) - iconInf.xHotspot;
    int infoContainerY = mousePos.y() + GetSystemMetrics(SM_CYCURSOR) - iconInf.yHotspot;
    if (infoContainerX + ui->widgetInfoContainer->width() > width()) {
        infoContainerX = mousePos.x() - GetSystemMetrics(SM_CXCURSOR) + iconInf.xHotspot - ui->widgetInfoContainer->width();
    }
    if (infoContainerY + ui->widgetInfoContainer->height() > height()) {
        infoContainerY = mousePos.y() - GetSystemMetrics(SM_CYCURSOR) + iconInf.yHotspot - ui->widgetInfoContainer->height();
    }
    if (iconInf.hbmMask) {
        DeleteObject(iconInf.hbmMask);
    }
    if (iconInf.hbmColor) {
        DeleteObject(iconInf.hbmColor);
    }

    ui->widgetInfoContainer->setVisible(true);
    ui->widgetInfoContainer->move(infoContainerX, infoContainerY);
    ui->labelPosition->setText("("
        + QString::number(devicePixelRatioMousePos.x() + 1) + ","
        + QString::number(devicePixelRatioMousePos.y() + 1)
        + ")");
    ui->labelSize->setText(
        QString::number(round(ui->frameRect->width() * devicePixelRatio)) + " x "
        + QString::number(round(ui->frameRect->height() * devicePixelRatio)));
    int r = 0, g = 0, b = 0;
    pixelColor.setRgb(
        image.pixelColor(devicePixelRatioMousePos.x(), devicePixelRatioMousePos.y())
            .rgb());
    pixelColor.getRgb(&r, &g, &b);
    ui->labelRGB->setText("RGB:(" + QString::number(r) + "," + QString::number(g) + "," + QString::number(b) + ")");

    // 更新预览图片---------------------------
    float scaledPreviewWidth = ui->labelPreview->width()
        / previewZoomRate;
    float scaledPreviewHeight = ui->labelPreview->height()
        / previewZoomRate;
    // 注意要向上取整ceil两次
    int x = ceil(devicePixelRatioMousePos.x() - (scaledPreviewWidth / devicePixelRatio / 2));
    int y = ceil(devicePixelRatioMousePos.y() - (scaledPreviewHeight / devicePixelRatio / 2));
    int x2 = ceil(devicePixelRatioMousePos.x() - (scaledPreviewWidth / 2));
    int y2 = ceil(devicePixelRatioMousePos.y() - (scaledPreviewHeight / 2));
    QPixmap processedPixmap(scaledPreviewWidth, scaledPreviewHeight);
    processedPixmap.setDevicePixelRatio(devicePixelRatio);
    QPainter painter(&processedPixmap);
    painter.setViewport(0, 0, scaledPreviewWidth, scaledPreviewHeight);
    // 开始绘制noScaledPreviewPixmap的位置
    int drawX = qMax(-x, 0);
    int drawY = qMax(-y, 0);
    // 绘制黑色填充背景
    painter.fillRect(0, 0, scaledPreviewWidth, scaledPreviewHeight, Qt::black);
    // 绘制noScaledPreviewPixmap
    painter.drawPixmap(drawX, drawY, pixmap.copy(x2, y2, scaledPreviewWidth, scaledPreviewHeight));
    painter.setPen(QPen(QBrush((QColor(7, 200, 250))), 1.0 / devicePixelRatio));
    painter.drawLine(
        0,
        round(processedPixmap.height() / devicePixelRatio / 2),
        round(processedPixmap.width() / devicePixelRatio),
        round(processedPixmap.height() / devicePixelRatio / 2));
    painter.drawLine(
        round(processedPixmap.width() / devicePixelRatio / 2),
        0,
        round(processedPixmap.width() / devicePixelRatio / 2),
        round(processedPixmap.height() / devicePixelRatio));
    ui->labelPreview->setPixmap(
        processedPixmap.scaled(
            round(ui->labelPreview->width() * devicePixelRatio),
            round(ui->labelPreview->height() * devicePixelRatio),
            Qt::KeepAspectRatio));
}

// 键盘事件
void RegionSelectorDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        auto devicePixelRatio = devicePixelRatioF();
        region = ui->frameRect->geometry();
        region.setX(region.x() * devicePixelRatio);
        region.setY(region.y() * devicePixelRatio);
        region.setBottom(region.bottom() * devicePixelRatio);
        region.setRight(region.right() * devicePixelRatio);
        qDebug() << region << devicePixelRatio;
        accept();
        return;
    } else if (event->key() == Qt::Key_Escape) {
        reject();
        return;
    }
}
