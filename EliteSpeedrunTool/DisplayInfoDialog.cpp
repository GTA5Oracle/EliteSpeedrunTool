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
    ui->labAct3Headshot->setVisible(displayInfoSubFunctions[DisplayInfoSubFunction::Act3Headshot]->display());
}

void DisplayInfoDialog::setTextAlignment()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labTimer->setAlignment(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->textAlignment());
    ui->labAct3Headshot->setAlignment(displayInfoSubFunctions[DisplayInfoSubFunction::Act3Headshot]->textAlignment());
}

void DisplayInfoDialog::setFont()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labTimer->setFont(
        QFont(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->fontFamily(),
            displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->textSize()));
    setTime(0, 0, 0); // 更新字体大小
    ui->labAct3Headshot->setFont(
        QFont(displayInfoSubFunctions[DisplayInfoSubFunction::Act3Headshot]->fontFamily(),
            displayInfoSubFunctions[DisplayInfoSubFunction::Act3Headshot]->textSize()));
    ui->labTimer->setText("0"); // 更新字体大小
}

void DisplayInfoDialog::setTextStyle()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    auto timerItem = displayInfoSubFunctions[DisplayInfoSubFunction::Timer];
    auto act3HeadshotItem = displayInfoSubFunctions[DisplayInfoSubFunction::Act3Headshot];

    setTextStyle(
        ui->labTimer,
        timerItem->textColor(),
        timerItem->textShadowColor(),
        timerItem->textShadowBlurRadius(),
        timerItem->textShadowOffset());
    setTextStyle(
        ui->labAct3Headshot,
        act3HeadshotItem->textColor(),
        act3HeadshotItem->textShadowColor(),
        act3HeadshotItem->textShadowBlurRadius(),
        act3HeadshotItem->textShadowOffset());
}

void DisplayInfoDialog::setTextStyle(
    QLabel* label,
    const QColor& textColor,
    const QColor& textShadowColor,
    qreal textShadowBlurRadius,
    const QPointF& textShadowOffset)
{
    label->setStyleSheet(
        textQssPattern.arg(textColor.name()));

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(textShadowColor);
    timerEffect->setBlurRadius(textShadowBlurRadius);
    timerEffect->setOffset(textShadowOffset);
    label->setGraphicsEffect(timerEffect);
}

void DisplayInfoDialog::setTouchable(bool touchable)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, !touchable);
}

void DisplayInfoDialog::setChildrenTransparentForMouseEvents(bool transparent)
{
    ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
    ui->labTimer->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
    ui->labAct3Headshot->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
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

void DisplayInfoDialog::setAct3Headshot(int headshot)
{
    ui->labAct3Headshot->setText(QString::number(headshot));
}

void DisplayInfoDialog::initGlobalDataConnects()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    auto timerItem = displayInfoSubFunctions[DisplayInfoSubFunction::Timer];
    auto act3HeadshotItem = displayInfoSubFunctions[DisplayInfoSubFunction::Act3Headshot];
    QList<QPair<DisplayInfoSubFunctionItem*, QLabel*>> items = {
        qMakePair(timerItem, ui->labTimer),
        qMakePair(act3HeadshotItem, ui->labAct3Headshot),
    };

    for (auto& item : items) {
        DisplayInfoSubFunctionItem* displayInfoSubFunctionItem = item.first;
        QLabel* label = item.second;
        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::displayChanged, this,
            [label](bool newDisplay) {
                label->setVisible(newDisplay);
            });

        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::textAlignmentChanged, this,
            [label](const Qt::Alignment& newTextAlignment) {
                label->setAlignment(newTextAlignment);
            });

        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::textSizeChanged, this,
            [displayInfoSubFunctionItem, label](int newTextSize) {
                label->setFont(QFont(displayInfoSubFunctionItem->fontFamily(), newTextSize));
            });

        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::textColorChanged, this,
            [this, displayInfoSubFunctionItem, label](const QColor& newTextColor) {
                setTextStyle(
                    label,
                    newTextColor,
                    displayInfoSubFunctionItem->textShadowColor(),
                    displayInfoSubFunctionItem->textShadowBlurRadius(),
                    displayInfoSubFunctionItem->textShadowOffset());
            });

        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::textShadowColorChanged, this,
            [this, displayInfoSubFunctionItem, label](const QColor& newTextShadowColor) {
                setTextStyle(
                    label,
                    displayInfoSubFunctionItem->textColor(),
                    newTextShadowColor,
                    displayInfoSubFunctionItem->textShadowBlurRadius(),
                    displayInfoSubFunctionItem->textShadowOffset());
            });

        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::textShadowBlurRadiusChanged, this,
            [this, displayInfoSubFunctionItem, label](int newTextShadowBlurRadius) {
                setTextStyle(
                    label,
                    displayInfoSubFunctionItem->textColor(),
                    displayInfoSubFunctionItem->textShadowColor(),
                    newTextShadowBlurRadius,
                    displayInfoSubFunctionItem->textShadowOffset());
            });

        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::textShadowOffsetChanged, this,
            [this, displayInfoSubFunctionItem, label](QPointF newTextShadowOffset) {
                setTextStyle(
                    label,
                    displayInfoSubFunctionItem->textColor(),
                    displayInfoSubFunctionItem->textShadowColor(),
                    displayInfoSubFunctionItem->textShadowBlurRadius(),
                    newTextShadowOffset);
            });

        connect(displayInfoSubFunctionItem, &DisplayInfoSubFunctionItem::fontFamilyChanged, this,
            [displayInfoSubFunctionItem, label](const QString& newFontFamily) {
                label->setFont(QFont(newFontFamily, displayInfoSubFunctionItem->textSize()));
            });
    }

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
