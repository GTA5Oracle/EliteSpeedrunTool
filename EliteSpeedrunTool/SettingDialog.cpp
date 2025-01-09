#include "SettingDialog.h"
#include "GlobalData.h"
#include "LanguageUtil.h"
#include "event/CmdEventHelper.h"
#include "event/cmd/CmdSequenceWizard.h"
#include "net/NetworkAdapterUtil.h"
#include <QColorDialog>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFont>
#include <QFontComboBox>
#include <QMenu>
#include <QPair>
#include <QScreen>
#include <QStyleFactory>
#include <QTimer>
#include <windows.h>

SettingDialog::SettingDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    {
        int i = 0;
        for (auto name : pageName) {
            QListWidgetItem* item = new QListWidgetItem(QIcon(pageIcon[i]), name, ui.lwPage);
            ui.lwPage->addItem(item);
            i++;
        }
    }
    connect(ui.lwPage, &QListWidget::currentRowChanged, ui.stackedWidget, &QStackedWidget::setCurrentIndex);

    initGeneralSettings();
    initFirewallSettings();
    initNetworkAdaptersSettings();
    initTimerSettings();
    initAct3Headshot();
    initSuspendProcess();
    initCloseGameImmediatelySettings();
    initHotkeyMapSettings();
    initEventCmdSettings();
    initSocialSettings();
    initLanguageSettings();
    initDevelopOptionsSettings();

    initDisplayInfoSettings();
    initRtssSettings();
    initRtssCrosshairSettings();
}

SettingDialog::~SettingDialog()
{
}

void SettingDialog::initGeneralSettings()
{
    // 最小化到托盘
    ui.cbMinimizeToTray->setChecked(globalData->minimizeToTray());
    connect(ui.cbMinimizeToTray, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->setMinimizeToTray(state == Qt::Checked);
    });

    // 自动检测更新
    ui.cbAutoCheckUpdate->setChecked(globalData->autoCheckUpdate());
    connect(ui.cbAutoCheckUpdate, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->setAutoCheckUpdate(state == Qt::Checked);
    });

    // Top most
    initHotkeyWidgets(
        globalData->topMostWindowHotkey(),
        ui.keySeqTopMostWindow,
        ui.tbClearTopMostWindowEdit,
        [](const QString& hotkey) { globalData->setTopMostWindowHotkey(hotkey); });

    // 样式
    for (auto&& k : QStyleFactory::keys()) {
        ui.cbStyle->addItem(k, k);
        if (k == globalData->styleName()) {
            ui.cbStyle->setCurrentText(k);
        }
    }
    connect(ui.cbStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        globalData->setStyleName(ui.cbStyle->itemData(index).toString());
    });
}

void SettingDialog::initSoundWidgets(
    const QString& soundPath,
    QLineEdit* soundPathEdit,
    QAbstractButton* playButton,
    QAbstractButton* selectFileButton,
    std::function<void(const QString&)> onUpdateSoundPath)
{
    soundPathEdit->setText(soundPath);
    connect(selectFileButton, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile(soundPath);
        if (!fileName.isEmpty()) {
            soundPathEdit->setText(fileName);
            onUpdateSoundPath(fileName);
        }
    });
    connect(soundPathEdit, &QLineEdit::textChanged, this, [onUpdateSoundPath](const QString& text) {
        onUpdateSoundPath(text);
    });
    connect(playButton, &QAbstractButton::clicked, this, [soundPathEdit]() {
        PlaySound(soundPathEdit->text().toStdWString().c_str(), nullptr, SND_FILENAME | SND_ASYNC);
    });
}

void SettingDialog::initHotkeyWidgets(
    const QString& hotkey,
    QKeySequenceEdit* keySequenceEdit,
    QAbstractButton* clearButton,
    std::function<void(const QString&)> onUpdateHotkey)
{
    keySequenceEdit->setKeySequence(QKeySequence(hotkey));
    connect(keySequenceEdit, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (keySequenceEdit->keySequence().count() > 1) {
            QKeyCombination value = keySequenceEdit->keySequence()[0];
            QKeySequence shortcut(value);
            keySequenceEdit->setKeySequence(shortcut);
            onUpdateHotkey(shortcut.toString());
        } else {
            onUpdateHotkey(keySequenceEdit->keySequence().toString());
        }
    });
    connect(clearButton, &QAbstractButton::clicked, this, [=]() {
        keySequenceEdit->clear();
        onUpdateHotkey("");
    });
}

QString SettingDialog::getSoundFile(QString dir)
{
    return QFileDialog::getOpenFileName(this, tr("选择音频文件"), dir, tr("WAV 文件 (*.wav)"));
}

// 防火墙
void SettingDialog::initFirewallSettings()
{
    // 快捷键
    initHotkeyWidgets(
        globalData->firewallStartHotkey(),
        ui.keySeqStartFirewall,
        ui.tbClearStartFirewallHotkeyEdit,
        [](const QString& hotkey) { globalData->setFirewallStartHotkey(hotkey); });
    initHotkeyWidgets(
        globalData->firewallStopHotkey(),
        ui.keySeqStopFirewall,
        ui.tbClearStopFirewallHotkeyEdit,
        [](const QString& hotkey) { globalData->setFirewallStopHotkey(hotkey); });

    // 音效
    initSoundWidgets(
        globalData->firewallStartSound(),
        ui.leFirewallStartSoundPath,
        ui.tbFirewallPlayStartSound,
        ui.tbFirewallSelectStartSound,
        [](const QString& fileName) { globalData->setFirewallStartSound(fileName); });
    initSoundWidgets(
        globalData->firewallStopSound(),
        ui.leFirewallStopSoundPath,
        ui.tbFirewallPlayStopSound,
        ui.tbFirewallSelectStopSound,
        [](const QString& fileName) { globalData->setFirewallStopSound(fileName); });
    initSoundWidgets(
        globalData->firewallErrorSound(),
        ui.leFirewallErrorSoundPath,
        ui.tbFirewallPlayErrorSound,
        ui.tbFirewallSelectErrorSound,
        [](const QString& fileName) { globalData->setFirewallErrorSound(fileName); });

    // 防火墙针对的exe程序
    ui.leFirewallAppPath->setText(globalData->firewallAppPath());
    connect(ui.tbSelectFirewallApp, &QAbstractButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("选择程序"),
            globalData->firewallAppPath(), tr("应用程序 (*.exe)"));
        if (!fileName.isEmpty()) {
            ui.leFirewallAppPath->setText(fileName);
            globalData->setFirewallAppPath(fileName);
        }
    });
    connect(ui.leFirewallAppPath, &QLineEdit::textChanged, this, [=](const QString& text) {
        globalData->setFirewallAppPath(text);
    });

    // 防火墙方向
    {
        QList<QPair<int, QString>> firewallDirections = {
            std::pair(0, tr("入站+出站")),
            std::pair(NET_FW_RULE_DIR_IN, tr("入站")),
            std::pair(NET_FW_RULE_DIR_OUT, tr("出站")),
        };
        int currentIndex = 0, i = 0;
        for (auto& direction : firewallDirections) {
            ui.cbFirewallDirection->addItem(direction.second, direction.first);
            if (direction.first == globalData->firewallDirection()) {
                currentIndex = i;
            }
            i++;
        }
        connect(ui.cbFirewallDirection, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                globalData->setFirewallDirection(ui.cbFirewallDirection->itemData(index).toInt());
            });
        ui.cbFirewallDirection->setCurrentIndex(currentIndex);
    }

    // 防火墙规则协议
    ui.sbFirewallProtocol->setValue(globalData->firewallProtocol());
    connect(ui.sbFirewallProtocol, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setFirewallProtocol(value);
    });
    connect(ui.tbFirewallProtocolWiki, &QAbstractButton::clicked, this, [=]() {
        QDesktopServices::openUrl(QUrl("https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml"));
    });
}

void SettingDialog::initNetworkAdaptersSettings()
{
    // 选择网络适配器
    auto adapters = networkAdapterUtil->networkAdapters();
    for (const auto& adapter : adapters) {
        auto item = new QListWidgetItem(adapter.name);
        item->setData(Qt::UserRole, adapter.deviceId);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(globalData->selectedNetworkAdapters().contains(adapter.deviceId) ? Qt::Checked : Qt::Unchecked);
        ui.lwNetworkAdapters->addItem(item);
    }
    connect(ui.lwNetworkAdapters, &QListWidget::itemChanged, this, [=](QListWidgetItem* item) {
        QList<QString> adapters;
        adapters.append(globalData->selectedNetworkAdapters());
        auto adapterName = item->data(Qt::UserRole).toString();
        if (item->checkState() == Qt::Checked) {
            if (!adapters.contains(adapterName)) {
                adapters.append(adapterName);
            }
        } else {
            if (adapters.contains(adapterName)) {
                adapters.removeAll(adapterName);
            }
        }
        globalData->setSelectedNetworkAdapters(adapters);
    });

    // 快捷键
    initHotkeyWidgets(
        globalData->networkAdaptersDisableHotkey(),
        ui.keySeqDisableNetworkAdapter,
        ui.tbClearDisableNetworkAdapterHotkeyEdit,
        [](const QString& hotkey) { globalData->setNetworkAdaptersDisableHotkey(hotkey); });
    initHotkeyWidgets(
        globalData->networkAdaptersEnableHotkey(),
        ui.keySeqEnableNetworkAdapter,
        ui.tbClearEnableNetworkAdapteHotkeyEdit,
        [](const QString& hotkey) { globalData->setNetworkAdaptersEnableHotkey(hotkey); });

    // 音效
    initSoundWidgets(
        globalData->networkAdaptersDisableSound(),
        ui.leDisableNetworkAdapterSoundPath,
        ui.tbPlayDisableNetworkAdapterSound,
        ui.tbSelectDisableNetworkAdapterSound,
        [](const QString& fileName) { globalData->setNetworkAdaptersDisableSound(fileName); });
    initSoundWidgets(
        globalData->networkAdaptersEnableSound(),
        ui.leEnableNetworkAdapterSoundPath,
        ui.tbPlayEnableNetworkAdapterSound,
        ui.tbSelectEnableNetworkAdapterSound,
        [](const QString& fileName) { globalData->setNetworkAdaptersEnableSound(fileName); });
}

// 计时器设置
void SettingDialog::initTimerSettings()
{
    initHotkeyWidgets(
        globalData->timerStartHotkey(),
        ui.keySeqStartTimer,
        ui.tbClearStartTimerHotkeyEdit,
        [](const QString& hotkey) { globalData->setTimerStartHotkey(hotkey); });
    initHotkeyWidgets(
        globalData->timerPauseHotkey(),
        ui.keySeqPauseTimer,
        ui.tbClearPauseTimerHotkeyEdit,
        [](const QString& hotkey) { globalData->setTimerPauseHotkey(hotkey); });
    initHotkeyWidgets(
        globalData->timerStopHotkey(),
        ui.keySeqStopTimer,
        ui.tbClearStopTimerHotkeyEdit,
        [](const QString& hotkey) { globalData->setTimerStopHotkey(hotkey); });

    // 手动计时刷新间隔
    ui.sbTimerUpdateInterval->setValue(globalData->timerUpdateInterval());
    connect(ui.sbTimerUpdateInterval, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setTimerUpdateInterval(value);
    });

    // 停止计时器策略
    {
        int i = 0;
        int currentTimerStopStrategyIndex = 0;
        for (auto s : globalData->timerStopStrategies()) {
            ui.cbTimerStopStrategy->addItem(TimerStopStrategyUtil::toDisplayString(s), s);
            if (s == globalData->timerStopStrategy()) {
                currentTimerStopStrategyIndex = i;
            }
            i++;
        }
        connect(ui.cbTimerStopStrategy, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                globalData->setTimerStopStrategy(
                    ui.cbTimerStopStrategy->itemData(index).value<TimerStopStrategy>());
            });
        ui.cbTimerStopStrategy->setCurrentIndex(currentTimerStopStrategyIndex);
    }
}

void SettingDialog::initDisplayInfoSettings()
{
    // 窗体 ============================================================
    RECT rect;
    rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) + rect.left;
    rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) + rect.top;
    // Tool Window
    ui.cbDisplayInfoToolWindow->setChecked(globalData->displayInfoToolWindow());
    connect(ui.cbDisplayInfoToolWindow, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->setDisplayInfoToolWindow(state == Qt::Checked);
    });
    // XY
    {
        auto displayInfoPos = globalData->displayInfoPos();
        ui.hsDisplayInfoX->setRange(0, qMax(rect.right - rect.left, displayInfoPos.x()));
        ui.hsDisplayInfoX->setValue(displayInfoPos.x());
        ui.hsDisplayInfoY->setRange(0, qMax(rect.bottom - rect.top, displayInfoPos.y()));
        ui.hsDisplayInfoY->setValue(displayInfoPos.y());
        ui.labDisplayInfoPos->setText(QString("%1, %2")
                                          .arg(displayInfoPos.x())
                                          .arg(displayInfoPos.y()));
    }
    connect(ui.hsDisplayInfoX, &QAbstractSlider::valueChanged, this, [=](int value) {
        globalData->setDisplayInfoPos({ value, globalData->displayInfoPos().y() });
        ui.labDisplayInfoPos->setText(QString("%1, %2")
                                          .arg(value)
                                          .arg(globalData->displayInfoPos().y()));
    });
    connect(ui.hsDisplayInfoY, &QAbstractSlider::valueChanged, this, [=](int value) {
        globalData->setDisplayInfoPos({ globalData->displayInfoPos().x(), value });
        ui.labDisplayInfoPos->setText(QString("%1, %2")
                                          .arg(globalData->displayInfoPos().x())
                                          .arg(value));
    });
    // 宽高
    {
        auto displayInfoSize = globalData->displayInfoSize();
        ui.hsDisplayInfoWidth->setRange(10, qMax(rect.right - rect.left, displayInfoSize.width()));
        ui.hsDisplayInfoWidth->setValue(displayInfoSize.width());
        ui.hsDisplayInfoHeight->setRange(10, qMax(rect.bottom - rect.top, displayInfoSize.height()));
        ui.hsDisplayInfoHeight->setValue(displayInfoSize.height());
        ui.labDisplayInfoSize->setText(QString("%1 x %2")
                                           .arg(displayInfoSize.width())
                                           .arg(displayInfoSize.height()));
    }
    connect(ui.hsDisplayInfoWidth, &QAbstractSlider::valueChanged, this, [=](int value) {
        globalData->setDisplayInfoSize({ value, globalData->displayInfoSize().height() });
        ui.labDisplayInfoSize->setText(QString("%1 x %2")
                                           .arg(value)
                                           .arg(globalData->displayInfoSize().height()));
    });
    connect(ui.hsDisplayInfoHeight, &QAbstractSlider::valueChanged, this, [=](int value) {
        globalData->setDisplayInfoSize({ globalData->displayInfoSize().width(), value });
        ui.labDisplayInfoSize->setText(QString("%1 x %2")
                                           .arg(globalData->displayInfoSize().width())
                                           .arg(value));
    });

    // 窗体背景颜色
    ui.labDisplayInfoBackground->setAutoFillBackground(true);
    QPalette paletteBackground = ui.labDisplayInfoBackground->palette();
    paletteBackground.setColor(QPalette::Window, globalData->displayInfoBackground().rgb());
    ui.labDisplayInfoBackground->setPalette(paletteBackground);
    connect(ui.tbSelectDisplayInfoBackground, &QAbstractButton::clicked, this, [=]() {
        QColorDialog dialog(globalData->displayInfoBackground());
        if (dialog.exec() == QDialog::Accepted) {
            auto c = QColor::fromRgb(dialog.selectedColor().rgb());
            c.setAlpha(globalData->displayInfoBackground().alpha());
            globalData->setDisplayInfoBackground(c);
            QPalette paletteBackground = ui.labDisplayInfoBackground->palette();
            paletteBackground.setColor(QPalette::Window, c.rgb());
            ui.labDisplayInfoBackground->setPalette(paletteBackground);
        }
    });
    ui.hsDisplayInfoAlpha->setRange(0, 255);
    ui.hsDisplayInfoAlpha->setValue(globalData->displayInfoBackground().alpha());
    ui.labDisplayInfoAlpha->setText(QString::number(globalData->displayInfoBackground().alpha()));
    connect(ui.hsDisplayInfoAlpha, &QAbstractSlider::valueChanged, this, [=](int value) {
        auto newBackgroundColor = QColor(globalData->displayInfoBackground());
        newBackgroundColor.setAlpha(value);
        globalData->setDisplayInfoBackground(newBackgroundColor);
        ui.labDisplayInfoAlpha->setText(QString::number(value));
    });

    // 内容 ================================================================================
    // 字体family
    connect(ui.fcbDisplayInfoFont, &QFontComboBox::currentFontChanged, this, [=](const QFont& font) {
        globalData->displayInfoSubFunctions()[currentSubFunction]->setFontFamily(font.family());
    });

    connect(ui.sbDisplayInfoTextSize, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->displayInfoSubFunctions()[currentSubFunction]->setTextSize(value);
    });

    connect(ui.tbSelectDisplayInfoTextColor, &QAbstractButton::clicked, this, [=]() {
        auto item = globalData->displayInfoSubFunctions()[currentSubFunction];
        QColorDialog dialog(item->textColor());
        if (dialog.exec() == QDialog::Accepted) {
            auto color = dialog.selectedColor();
            item->setTextColor(color);
            ui.labDisplayInfoTextColor->setStyleSheet(
                QString("background-color: %1;").arg(color.name()));
        }
    });

    // 阴影
    connect(ui.sbDisplayInfoTextShadowBlurRadius, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->displayInfoSubFunctions()[currentSubFunction]->setTextShadowBlurRadius(value);
    });
    connect(ui.sbDisplayInfoTextShadowOffsetX, &QSpinBox::valueChanged, this, [=](int value) {
        auto item = globalData->displayInfoSubFunctions()[currentSubFunction];
        item->setTextShadowOffset({ (float)value, item->textShadowOffset().y() });
    });
    connect(ui.sbDisplayInfoTextShadowOffsetY, &QSpinBox::valueChanged, this, [=](int value) {
        auto item = globalData->displayInfoSubFunctions()[currentSubFunction];
        item->setTextShadowOffset({ item->textShadowOffset().x(), (float)value });
    });
    connect(ui.tbSelectDisplayInfoTextShadowColor, &QAbstractButton::clicked, this, [=]() {
        auto item = globalData->displayInfoSubFunctions()[currentSubFunction];
        QColorDialog dialog(item->textShadowColor());
        if (dialog.exec() == QDialog::Accepted) {
            auto color = dialog.selectedColor();
            item->setTextShadowColor(color);
            ui.labDisplayInfoTextShadowColor->setStyleSheet(
                QString("background-color: %1;").arg(color.name()));
        }
    });

    // 内容对齐
    for (int i = 0; i < horiAlign.count(); i++) {
        ui.cbDisplayInfoTextHAlign->addItem(horiAlign[i].second, horiAlign[i].first);
    }
    connect(ui.cbDisplayInfoTextHAlign, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            auto currentSetting = globalData->displayInfoSubFunctions()[currentSubFunction];
            currentSetting->setTextAlignment(Qt::Alignment(
                ui.cbDisplayInfoTextHAlign->itemData(index).toInt()
                | currentSetting->textAlignment() & Qt::AlignVertical_Mask));
        });

    for (int i = 0; i < vertAlign.count(); i++) {
        ui.cbDisplayInfoTextVAlign->addItem(vertAlign[i].second, vertAlign[i].first);
    }
    connect(ui.cbDisplayInfoTextVAlign, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            auto currentSetting = globalData->displayInfoSubFunctions()[currentSubFunction];
            currentSetting->setTextAlignment(Qt::Alignment(
                ui.cbDisplayInfoTextVAlign->itemData(index).toInt()
                | currentSetting->textAlignment() & Qt::AlignHorizontal_Mask));
        });

    // 在窗口展示
    connect(ui.cbDisplayInfoFuncEnable, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->displayInfoSubFunctions()[currentSubFunction]->setDisplay(state == Qt::Checked);
    });

    // 设置分类
    for (auto f : globalData->funcs()) {
        ui.cbDisplayInfoFunction->addItem(DisplayInfoSubFunctionUtil::toDisplayString(f), f);
    }
    ui.cbDisplayInfoFunction->removeItem(0); // 暂时不显示防火墙
    connect(ui.cbDisplayInfoFunction, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            currentSubFunctionIndex = index;
            currentSubFunction = ui.cbDisplayInfoFunction->itemData(index).value<DisplayInfoSubFunction>();
            setDisplayInfoSubFunctionSettings(currentSubFunction);
        });
    setDisplayInfoSubFunctionSettings(currentSubFunction);
    ui.cbDisplayInfoFunction->setCurrentIndex(currentSubFunctionIndex);
}

void SettingDialog::initRtssSettings()
{
    ui.cbRtssOverlay->setChecked(globalData->rtssOverlay());
    ui.gbRtssSubFunction->setEnabled(globalData->rtssOverlay());
    connect(ui.cbRtssOverlay, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->setRtssOverlay(state == Qt::Checked);
        ui.gbRtssSubFunction->setEnabled(state == Qt::Checked);
    });

    // 在OSD上展示
    connect(ui.cbRtssSubFunctionOsd, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->displayInfoSubFunctions()[currentRtssSubFunction]->setRtssDisplay(state == Qt::Checked);
    });
    // OSD文本
    connect(ui.teRtssSubFunctionOsdText, &QPlainTextEdit::textChanged, this, [=]() {
        globalData->displayInfoSubFunctions()[currentRtssSubFunction]->setRtssOsdText(ui.teRtssSubFunctionOsdText->toPlainText());
    });
    // 重置OSD文本
    connect(ui.tbRtssSubFunctionResetOsdText, &QAbstractButton::clicked, this, [=]() {
        ui.teRtssSubFunctionOsdText->setPlainText(DisplayInfoSubFunctionUtil::defaultRtssOsdText(currentRtssSubFunction));
    });

    // 设置子项
    for (auto f : globalData->funcs()) {
        ui.cbRtssSubFunction->addItem(DisplayInfoSubFunctionUtil::toDisplayString(f), f);
    }
    ui.cbRtssSubFunction->removeItem(0); // 暂时不显示防火墙
    connect(ui.cbRtssSubFunction, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        currentRtssSubFunctionIndex = index;
        currentRtssSubFunction = ui.cbRtssSubFunction->itemData(index).value<DisplayInfoSubFunction>();
        setRtssSubFunctionSettings(currentRtssSubFunction);
    });
    setRtssSubFunctionSettings(currentRtssSubFunction);
    ui.cbRtssSubFunction->setCurrentIndex(currentRtssSubFunctionIndex);
}

void SettingDialog::initRtssCrosshairSettings()
{
    ui.cbRtssCrosshair->setChecked(globalData->rtssCrosshair());
    connect(ui.cbRtssCrosshair, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->setRtssCrosshair(state == Qt::Checked);
    });

    // Text
    ui.leRtssCrosshairText->setText(globalData->rtssCrosshairText());
    connect(ui.leRtssCrosshairText, &QLineEdit::textChanged, this, [=](const QString& text) {
        globalData->setRtssCrosshairText(text);
    });

    // XY
    ui.sbRtssCrosshairX->setValue(globalData->rtssCrosshairX());
    connect(ui.sbRtssCrosshairX, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setRtssCrosshairX(value);
    });
    ui.sbRtssCrosshairY->setValue(globalData->rtssCrosshairY());
    connect(ui.sbRtssCrosshairY, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setRtssCrosshairY(value);
    });

    // Size
    ui.sbRtssCrosshairSize->setValue(globalData->rtssCrosshairSize());
    connect(ui.sbRtssCrosshairSize, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setRtssCrosshairSize(value);
    });

    // Color
    ui.labRtssCrosshairColor->setAutoFillBackground(true);
    QPalette paletteBackground = ui.labRtssCrosshairColor->palette();
    paletteBackground.setColor(QPalette::Window, globalData->rtssCrosshairColor().rgb());
    ui.labRtssCrosshairColor->setPalette(paletteBackground);
    connect(ui.tbRtssCrosshairColor, &QAbstractButton::clicked, this, [=]() {
        QColorDialog dialog(globalData->rtssCrosshairColor());
        if (dialog.exec() == QDialog::Accepted) {
            auto color = dialog.selectedColor();
            globalData->setRtssCrosshairColor(color);
            ui.labRtssCrosshairColor->setStyleSheet(QString("background-color: %1;").arg(color.name()));
        }
    });
}

void SettingDialog::initAct3Headshot()
{
    initHotkeyWidgets(
        globalData->act3HeadshotStartHotkey(),
        ui.keySeqAct3HeadshotStart,
        ui.tbClearAct3HeadshotStart,
        [](const QString& hotkey) { globalData->setAct3HeadshotStartHotkey(hotkey); });
    initHotkeyWidgets(
        globalData->act3HeadshotStopHotkey(),
        ui.keySeqAct3HeadshotStop,
        ui.tbClearAct3HeadshotStop,
        [](const QString& hotkey) { globalData->setAct3HeadshotStopHotkey(hotkey); });

    // 音效
    initSoundWidgets(
        globalData->act3HeadshotStartSound(),
        ui.leAct3HeadshotStartSoundPath,
        ui.tbAct3HeadshotPlayStartSound,
        ui.tbAct3HeadshotStartSound,
        [](const QString& fileName) { globalData->setAct3HeadshotStartSound(fileName); });
    initSoundWidgets(
        globalData->act3HeadshotStopSound(),
        ui.leAct3HeadshotStopSoundPath,
        ui.tbAct3HeadshotPlayStopSound,
        ui.tbAct3HeadshotStopSound,
        [](const QString& fileName) { globalData->setAct3HeadshotStopSound(fileName); });
}

void SettingDialog::initSuspendProcess()
{
    initHotkeyWidgets(
        globalData->suspendAndResumeHotkey(),
        ui.keySeqSuspendProcess,
        ui.tbClearSuspendProcessEdit,
        [](const QString& hotkey) { globalData->setSuspendAndResumeHotkey(hotkey); });

    ui.sbSuspendProcessDuration->setValue(globalData->suspendAndResumeDuration());
    connect(ui.sbSuspendProcessDuration, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setSuspendAndResumeDuration(value);
    });
}

void SettingDialog::initCloseGameImmediatelySettings()
{
    initHotkeyWidgets(
        globalData->closeGameImmediatelyHotkey(),
        ui.keySeqCloseGameImmediately,
        ui.tbClearCloseGameImmediatelyHotkeyEdit,
        [](const QString& hotkey) { globalData->setCloseGameImmediatelyHotkey(hotkey); });
}

void SettingDialog::initHotkeyMapSettings()
{
    for (auto& key : globalData->hotkeyMaps()) {
        ui.twHotkeyMap->addNewRow(key);
    }
    connect(ui.twHotkeyMap, &KeySequenceTableWidget::onAdd, this, [](QKeySequence keySeq) {
        QString keyStr = keySeq.toString();
        auto newList = QList(globalData->hotkeyMaps());
        if (!newList.contains(keyStr)) {
            globalData->setHotkeyMaps(newList << keyStr);
        }
    });
    connect(ui.twHotkeyMap, &KeySequenceTableWidget::onDelete, this, [](QKeySequence keySeq) {
        QString keyStr = keySeq.toString();
        auto newList = QList(globalData->hotkeyMaps());
        newList.removeAll(keyStr);
        globalData->setHotkeyMaps(newList);
    });
    connect(ui.twHotkeyMap, &KeySequenceTableWidget::onChanged, this, [](QKeySequence oldKeySeq, QKeySequence newKeySeq) {
        QString oldKeyStr = oldKeySeq.toString();
        QString newKeyStr = newKeySeq.toString();
        auto newList = QList(globalData->hotkeyMaps());
        newList.removeAll(oldKeyStr);
        if (!newList.contains(newKeyStr)) {
            globalData->setHotkeyMaps(newList << newKeyStr);
        }
        globalData->setHotkeyMaps(newList);
    });
    connect(ui.tbHotkeyMapAdd, &QAbstractButton::clicked, this, [this]() {
        ui.twHotkeyMap->addNewRow(QKeySequence());
    });
}

void SettingDialog::initEventCmdSettings()
{
    connect(ui.teEventCmd, &QPlainTextEdit::textChanged, this, [=]() {
        const auto& name = ui.cbEventCmd->itemData(currentEventCmdIndex).toString();
        globalData->eventCmdMap()[name] = ui.teEventCmd->toPlainText();
    });

    connect(ui.tbEventCmdWizard, &QAbstractButton::clicked, this, [this]() {
        CmdSequenceWizard wizard(ui.cbEventCmd->currentText());
        if (wizard.exec() == QDialog::Accepted) {
            if (wizard.insertModeReplace) {
                ui.teEventCmd->setPlainText(wizard.cmdSequence() + "\n");
            } else {
                auto old = ui.teEventCmd->toPlainText();
                if (old.endsWith("\n")) {
                    ui.teEventCmd->setPlainText(old + wizard.cmdSequence() + "\n");
                } else {
                    ui.teEventCmd->setPlainText(old + "\n" + wizard.cmdSequence() + "\n");
                }
            }
        }
    });

    const auto& eventNameMap = cmdEventHelper->getEventNameMap();
    for (auto i = eventNameMap.constBegin(); i != eventNameMap.constEnd(); i++) {
        ui.cbEventCmd->addItem(i.value(), i.key());
    }
    connect(ui.cbEventCmd, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        currentEventCmdIndex = index;
        auto name = ui.cbEventCmd->itemData(index).toString();
        setEventCmdSubSettings(name);
    });
    setEventCmdSubSettings(ui.cbEventCmd->itemData(currentEventCmdIndex).toString());
}

void SettingDialog::initSocialSettings()
{
    ui.cbDiscordShowRp->setChecked(globalData->discordShowRichPresence());
    connect(ui.cbDiscordShowRp, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        ui.cbDiscordShowRp->setEnabled(false);
        QTimer::singleShot(1000, this, [this]() {
            ui.cbDiscordShowRp->setEnabled(true);
        });
        globalData->setDiscordShowRichPresence(state == Qt::Checked);
    });

    connect(ui.tbDiscordRpHelp, &QAbstractButton::clicked, this, [=](int state) {
        QDesktopServices::openUrl(QUrl("https://discord.com/developers/docs/rich-presence/how-to"));
    });
}

void SettingDialog::initLanguageSettings()
{
    for (auto l : LanguageUtil::getInstance()->languages) {
        ui.cbLanguage->addItem(LanguageUtil::getDisplayName(l.name), l.name);
        if (l.name == globalData->language()) {
            ui.cbLanguage->setCurrentIndex(ui.cbLanguage->count() - 1);
        }
    }
    connect(ui.cbLanguage, &QComboBox::activated, this, [=]() {
        globalData->setLanguage(ui.cbLanguage->currentData().toString());
        LanguageUtil::applyLanguage();
        ui.retranslateUi(this);
    });
}

void SettingDialog::initDevelopOptionsSettings()
{
    ui.cbDebugMode->setChecked(globalData->debugMode());
    connect(ui.cbDebugMode, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        globalData->setDebugMode(state == Qt::Checked);
    });
}

void SettingDialog::setDisplayInfoSubFunctionSettings(DisplayInfoSubFunction f)
{
    auto currentSetting = globalData->displayInfoSubFunctions()[f];
    ui.cbDisplayInfoFuncEnable->setChecked(currentSetting->display());
    ui.fcbDisplayInfoFont->setCurrentFont(QFont(currentSetting->fontFamily()));
    ui.sbDisplayInfoTextSize->setValue(currentSetting->textSize());
    ui.labDisplayInfoTextColor->setStyleSheet(
        QString("background-color: %1;").arg(currentSetting->textColor().name()));
    ui.sbDisplayInfoTextShadowBlurRadius->setValue(currentSetting->textShadowBlurRadius());
    ui.sbDisplayInfoTextShadowOffsetX->setValue(currentSetting->textShadowOffset().x());
    ui.sbDisplayInfoTextShadowOffsetY->setValue(currentSetting->textShadowOffset().y());
    ui.labDisplayInfoTextShadowColor->setStyleSheet(
        QString("background-color: %1;").arg(currentSetting->textShadowColor().name()));
    for (int i = 0; i < horiAlign.count(); i++) {
        if (horiAlign[i].first == (currentSetting->textAlignment() & Qt::AlignHorizontal_Mask).toInt()) {
            ui.cbDisplayInfoTextHAlign->setCurrentIndex(i);
        }
    }
    for (int i = 0; i < vertAlign.count(); i++) {
        if (vertAlign[i].first == (currentSetting->textAlignment() & Qt::AlignVertical_Mask).toInt()) {
            ui.cbDisplayInfoTextVAlign->setCurrentIndex(i);
        }
    }
}

void SettingDialog::setRtssSubFunctionSettings(DisplayInfoSubFunction f)
{
    auto currentSetting = globalData->displayInfoSubFunctions()[f];
    ui.cbRtssSubFunctionOsd->setChecked(currentSetting->rtssDisplay());
    ui.teRtssSubFunctionOsdText->setPlainText(currentSetting->rtssOsdText());
}

void SettingDialog::setEventCmdSubSettings(QString name)
{
    ui.teEventCmd->setPlainText(globalData->eventCmdMap()[name]);
}
