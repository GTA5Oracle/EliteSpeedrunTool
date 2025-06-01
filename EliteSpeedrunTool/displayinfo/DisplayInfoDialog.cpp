#include "DisplayInfoDialog.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include "ui_DisplayInfoDialog.h"

#include "SubFuncsData.h"
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

    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    items = {
        qMakePair(displayInfoSubFunctions[DisplayInfoSubFunction::Firewall], ui->labFirewall),
        qMakePair(displayInfoSubFunctions[DisplayInfoSubFunction::Timer], ui->labTimer),
        // qMakePair(displayInfoSubFunctions[DisplayInfoSubFunction::AutoTimer], ui->labAutoTimer),
        qMakePair(displayInfoSubFunctions[DisplayInfoSubFunction::Act3Headshot], ui->labAct3Headshot),
    };
    ui->labAutoTimer->setVisible(false);

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

    initGlobalDataConnects();

    initDisplayData();
    connect(subFuncsData, &SubFuncsData::elementChanged, this, [this](const DisplayInfoSubFunction& key, const QVariant& value) {
        subFuncs[key](value);
    });
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
    for (auto& item : items) {
        DisplayInfoSubFunctionItem* displayInfoSubFunctionItem = item.first;
        QLabel* label = item.second;
        label->setVisible(displayInfoSubFunctionItem->display());
    }
}

void DisplayInfoDialog::setTextAlignment()
{
    for (auto& item : items) {
        DisplayInfoSubFunctionItem* displayInfoSubFunctionItem = item.first;
        QLabel* label = item.second;
        label->setAlignment(displayInfoSubFunctionItem->textAlignment());
    }
}

void DisplayInfoDialog::setFont()
{
    for (auto& item : items) {
        DisplayInfoSubFunctionItem* displayInfoSubFunctionItem = item.first;
        QLabel* label = item.second;
        label->setFont(QFont(displayInfoSubFunctionItem->fontFamily(), displayInfoSubFunctionItem->textSize()));
    }
    initDisplayData(); // 更新字体大小
}

void DisplayInfoDialog::setTextStyle()
{
    for (auto& item : items) {
        DisplayInfoSubFunctionItem* displayInfoSubFunctionItem = item.first;
        QLabel* label = item.second;
        setTextStyle(
            label,
            displayInfoSubFunctionItem->textColor(),
            displayInfoSubFunctionItem->textShadowColor(),
            displayInfoSubFunctionItem->textShadowBlurRadius(),
            displayInfoSubFunctionItem->textShadowOffset());
    }
}

void DisplayInfoDialog::setTextStyle(
    QLabel* label,
    const QColor& textColor,
    const QColor& textShadowColor,
    qreal textShadowBlurRadius,
    const QPointF& textShadowOffset)
{
    label->setStyleSheet(textQssPattern.arg(textColor.name()));

    QGraphicsDropShadowEffect* textEffect = new QGraphicsDropShadowEffect(this);
    textEffect->setColor(textShadowColor);
    textEffect->setBlurRadius(textShadowBlurRadius);
    textEffect->setOffset(textShadowOffset);
    label->setGraphicsEffect(textEffect);
}

void DisplayInfoDialog::setTouchable(bool touchable)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, !touchable);
}

void DisplayInfoDialog::setChildrenTransparentForMouseEvents(bool transparent)
{
    ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
    for (auto& item : items) {
        QLabel* label = item.second;
        label->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
    }
}

void DisplayInfoDialog::initDisplayData()
{
    subFuncsData->forEach([this](const DisplayInfoSubFunction& key, const QVariant& value) {
        if (subFuncs.contains(key)) {
            subFuncs[key](value);
        }
    });
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

void DisplayInfoDialog::setFirewall(QVariant v)
{
    ui->labFirewall->setVisible(globalData->displayInfoSubFunctions()[DisplayInfoSubFunction::Firewall]->display()
        && (v.toBool() || globalData->firewallAlwaysDisplay()));
    ui->labFirewall->setText(tr("防火墙%1").arg(v.toBool() ? tr("开") : tr("关")));
}

void DisplayInfoDialog::setTime(QVariant v)
{
    auto deltaTime = v.toULongLong();
    int m = deltaTime / 1000 / 60;
    int s = (deltaTime / 1000) % 60;
    int ms = (deltaTime % 1000) / 10;
    auto textSize = globalData->displayInfoSubFunctions()[DisplayInfoSubFunction::Timer]->textSize();
    ui->labTimer->setText(timePattern
                              .arg(QString::number(textSize))
                              .arg(m, 2, 10, QLatin1Char('0'))
                              .arg(s, 2, 10, QLatin1Char('0'))
                              .arg(QString::number(qMax(5, textSize - 10)))
                              .arg(ms, 2, 10, QLatin1Char('0')));
}

void DisplayInfoDialog::setAutoTime(QVariant v)
{
    auto deltaTime = v.toULongLong();
    int m = deltaTime / 1000 / 60;
    int s = (deltaTime / 1000) % 60;
    int ms = (deltaTime % 1000) / 10;
    auto textSize = globalData->displayInfoSubFunctions()[DisplayInfoSubFunction::AutoTimer]->textSize();
    ui->labAutoTimer->setText(timePattern
                                  .arg(QString::number(textSize))
                                  .arg(m, 2, 10, QLatin1Char('0'))
                                  .arg(s, 2, 10, QLatin1Char('0'))
                                  .arg(QString::number(qMax(5, textSize - 10)))
                                  .arg(ms, 2, 10, QLatin1Char('0')));
}

void DisplayInfoDialog::setAct3Headshot(QVariant v)
{
    ui->labAct3Headshot->setText(QString::number(v.toInt()));
}

void DisplayInfoDialog::initGlobalDataConnects()
{
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

    // ==========================
    connect(globalData, &GlobalData::firewallAlwaysDisplayChanged, this, [this]() {
        ui->labFirewall->setVisible(globalData->displayInfoSubFunctions()[DisplayInfoSubFunction::Firewall]->display()
            && (subFuncsData->value(DisplayInfoSubFunction::Firewall).toBool() || globalData->firewallAlwaysDisplay()));
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
