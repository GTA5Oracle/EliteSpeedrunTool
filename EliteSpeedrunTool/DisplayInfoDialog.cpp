#include "DisplayInfoDialog.h"
#include "GlobalData.h"
#include "ui_DisplayInfoDialog.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QTimer>
#include <windows.h>

const QString DisplayInfoDialog::timePattern = "<html><head/><body><p><span style=\" font-size:%1pt;\">%2:%3</span><span style=\" font-size:%4pt;\">.%5</span></p></body></html>";
const QString DisplayInfoDialog::textQssPattern = "color: %1;";
DisplayInfoDialog::DisplayInfoDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DisplayInfoDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setCursor(Qt::OpenHandCursor);

    setTouchable();
    setChildrenTransparentForMouseEvents();
    setDisplay();
    setTextAlignment();
    auto windowFlag = globalData->displayInfoToolWindow()
        ? Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
        : Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
    setWindowFlags(windowFlag);
    qreal devicePixelRatio = screen()->devicePixelRatio();
    setGeometry(QRect(globalData->displayInfoPos() / devicePixelRatio, globalData->displayInfoSize() / devicePixelRatio));

    setDialogBackground();
    setFont();
    setTextStyle();

    setTime(0, 0, 0);

    initGlobalDataConnects();
}

DisplayInfoDialog::~DisplayInfoDialog()
{
    globalData->setDisplayInfoSize({ width(), height() });
    delete ui;
}

void DisplayInfoDialog::setDialogBackground(QColor color)
{
    ui->widget->setStyleSheet("#widget { background-color: " + color.name(QColor::HexArgb) + "; }");
}

void DisplayInfoDialog::setDisplay()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labTimer->setVisible(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->display());
}

void DisplayInfoDialog::setTextAlignment()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labTimer->setAlignment(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->textAlignment());
}

void DisplayInfoDialog::setFont()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labTimer->setFont(
        QFont(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->fontFamily(),
            displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->textSize()));
    setTime(0, 0, 0); // 更新字体大小
}

void DisplayInfoDialog::setTextStyle()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    auto timerItem = displayInfoSubFunctions[DisplayInfoSubFunction::Timer];

    setTimerTextStyle(
        timerItem->textColor(),
        timerItem->textShadowColor(),
        timerItem->textShadowBlurRadius(),
        timerItem->textShadowOffset());
}

void DisplayInfoDialog::setTimerTextStyle(
    const QColor& textColor,
    const QColor& textShadowColor,
    qreal textShadowBlurRadius,
    const QPointF& textShadowOffset)
{
    ui->labTimer->setStyleSheet(
        textQssPattern.arg(textColor.name()));

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(textShadowColor);
    timerEffect->setBlurRadius(textShadowBlurRadius);
    timerEffect->setOffset(textShadowOffset);
    ui->labTimer->setGraphicsEffect(timerEffect);
}

void DisplayInfoDialog::setTouchable(bool touchable)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, !touchable);
}

void DisplayInfoDialog::setChildrenTransparentForMouseEvents(bool transparent)
{
    ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
    ui->labTimer->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
}

void DisplayInfoDialog::addWidget(QWidget* widget)
{
    ui->mainLayout->addWidget(widget);
    ui->mainLayout->update();
    insertedWidget << widget;
}

void DisplayInfoDialog::removeWidget(QWidget* widget)
{
    ui->mainLayout->removeWidget(widget);
    // qDebug() << widget->parent() << widget->parentWidget() << ui->mainLayout << this;
    if (widget->parent() == ui->widget) { // 这里就是== ui->widget没错Layout不能作为单独的parent
        widget->setParent(nullptr);
    }
    ui->mainLayout->update();
    insertedWidget.removeOne(widget);
}

void DisplayInfoDialog::removeAllInsertedWidget()
{
    for (auto w : insertedWidget) {
        DisplayInfoDialog::removeWidget(w);
    }
}

bool DisplayInfoDialog::containWidget(QWidget* widget)
{
    return ui->mainLayout->children().contains(widget);
}

int DisplayInfoDialog::widgetCount()
{
    return ui->mainLayout->count();
}

void DisplayInfoDialog::setTime(unsigned int m, unsigned int s, unsigned int ms)
{
    auto textSize = globalData->displayInfoSubFunctions()[DisplayInfoSubFunction::Timer]->textSize();
    ui->labTimer->setText(timePattern
                              .arg(QString::number(textSize))
                              .arg(m, 2, 10, QLatin1Char('0'))
                              .arg(s, 2, 10, QLatin1Char('0'))
                              .arg(QString::number(qMax(5, textSize - 10)))
                              .arg(ms, 2, 10, QLatin1Char('0')));
}

void DisplayInfoDialog::initGlobalDataConnects()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    auto timerItem = displayInfoSubFunctions[DisplayInfoSubFunction::Timer];

    connect(timerItem, &DisplayInfoSubFunctionItem::displayChanged, this,
        [this](bool newDisplay) {
            ui->labTimer->setVisible(newDisplay);
        });

    connect(timerItem, &DisplayInfoSubFunctionItem::textAlignmentChanged, this,
        [this](const Qt::Alignment& newTextAlignment) {
            ui->labTimer->setAlignment(newTextAlignment);
        });

    connect(timerItem, &DisplayInfoSubFunctionItem::textSizeChanged, this,
        [this, timerItem](int newTextSize) {
            ui->labTimer->setFont(QFont(timerItem->fontFamily(), newTextSize));
        });

    connect(timerItem, &DisplayInfoSubFunctionItem::textColorChanged, this,
        [this, timerItem](const QColor& newTextColor) {
            setTimerTextStyle(
                newTextColor,
                timerItem->textShadowColor(),
                timerItem->textShadowBlurRadius(),
                timerItem->textShadowOffset());
        });

    connect(timerItem, &DisplayInfoSubFunctionItem::textShadowColorChanged, this,
        [this, timerItem](const QColor& newTextShadowColor) {
            setTimerTextStyle(
                timerItem->textColor(),
                newTextShadowColor,
                timerItem->textShadowBlurRadius(),
                timerItem->textShadowOffset());
        });

    connect(timerItem, &DisplayInfoSubFunctionItem::textShadowBlurRadiusChanged, this,
        [this, timerItem](int newTextShadowBlurRadius) {
            setTimerTextStyle(
                timerItem->textColor(),
                timerItem->textShadowColor(),
                newTextShadowBlurRadius,
                timerItem->textShadowOffset());
        });

    connect(timerItem, &DisplayInfoSubFunctionItem::textShadowOffsetChanged, this,
        [this, timerItem](QPointF newTextShadowOffset) {
            setTimerTextStyle(
                timerItem->textColor(),
                timerItem->textShadowColor(),
                timerItem->textShadowBlurRadius(),
                newTextShadowOffset);
        });

    connect(timerItem, &DisplayInfoSubFunctionItem::fontFamilyChanged, this,
        [this, timerItem](const QString& newFontFamily) {
            ui->labTimer->setFont(QFont(newFontFamily, timerItem->textSize()));
        });

    // ==========================
    connect(globalData, &GlobalData::displayInfoPosChanged, this, [this]() {
        move(globalData->displayInfoPos() / screen()->devicePixelRatio());
    });
    connect(globalData, &GlobalData::displayInfoSizeChanged, this, [this]() {
        qreal devicePixelRatio = screen()->devicePixelRatio();
        setGeometry(QRect(
            globalData->displayInfoPos() / devicePixelRatio,
            globalData->displayInfoSize() / devicePixelRatio));
    });
    connect(globalData, &GlobalData::displayInfoBackgroundChanged, this, [this]() {
        setDialogBackground();
    });
}

void DisplayInfoDialog::mousePressEvent(QMouseEvent* event)
{
    mousePressedPos = event->pos();
    setCursor(Qt::ClosedHandCursor);
    QDialog::mousePressEvent(event);
}

void DisplayInfoDialog::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(Qt::OpenHandCursor);
    qreal devicePixelRatio = screen()->devicePixelRatio();
    globalData->setDisplayInfoPos({ static_cast<int>(x() * devicePixelRatio),
        static_cast<int>(y() * devicePixelRatio) });
    QDialog::mouseReleaseEvent(event);
}

void DisplayInfoDialog::mouseMoveEvent(QMouseEvent* event)
{
    move((event->globalPosition() - mousePressedPos).toPoint());
    QDialog::mouseMoveEvent(event);
}

void DisplayInfoDialog::closeEvent(QCloseEvent* event)
{
    removeAllInsertedWidget();
    QDialog::closeEvent(event);
}
