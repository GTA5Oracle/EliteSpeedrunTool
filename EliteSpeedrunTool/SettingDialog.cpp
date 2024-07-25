#include "SettingDialog.h"
#include "GlobalData.h"
#include "LanguageUtil.h"
#include "dataobserver/missionstrategy/MissionStrategyUtil.h"
#include <QColorDialog>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFont>
#include <QFontComboBox>
#include <QPair>
#include <QScreen>
#include <QStyleFactory>
#include <QTimer>
#include <dataobserver/datafetcher/DataFetcherUtil.h>
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
    initMissionDataSettings();
    initTimerSettings();
    initCloseGameImmediatelySettings();
    initSocialSettings();
    initLanguageSettings();
    initDevelopOptionsSettings();

    initDisplayInfoSettings();
}

SettingDialog::~SettingDialog()
{
}

void SettingDialog::initGeneralSettings()
{
    // 最小化到托盘
    ui.cbMinimizeToTray->setChecked(globalData->minimizeToTray());
    connect(ui.cbMinimizeToTray, &QCheckBox::stateChanged, this, [=](int state) {
        globalData->setMinimizeToTray(state == Qt::Checked);
    });

    // 自动检测更新
    ui.cbAutoCheckUpdate->setChecked(globalData->autoCheckUpdate());
    connect(ui.cbAutoCheckUpdate, &QCheckBox::stateChanged, this, [=](int state) {
        globalData->setAutoCheckUpdate(state == Qt::Checked);
    });

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

QString SettingDialog::getSoundFile(QString dir)
{
    return QFileDialog::getOpenFileName(this, tr("选择文件"), dir, tr("WAV 文件 (*.wav)"));
}

// 防火墙
void SettingDialog::initFirewallSettings()
{
    // 热键
    ui.keySeqStartFirewall->setKeySequence(QKeySequence(globalData->firewallStartHotkey()));
    connect(ui.keySeqStartFirewall, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStartFirewall->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStartFirewall->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStartFirewall->setKeySequence(shortcut);
            globalData->setFirewallStartHotkey(shortcut.toString());
        } else {
            globalData->setFirewallStartHotkey(ui.keySeqStartFirewall->keySequence().toString());
        }
    });

    connect(ui.tbClearStartFirewallHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStartFirewall->clear();
        globalData->setFirewallStartHotkey("");
    });

    ui.keySeqStopFirewall->setKeySequence(QKeySequence(globalData->firewallStopHotkey()));
    connect(ui.keySeqStopFirewall, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStopFirewall->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStopFirewall->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStopFirewall->setKeySequence(shortcut);
            globalData->setFirewallStopHotkey(shortcut.toString());
        } else {
            globalData->setFirewallStopHotkey(ui.keySeqStopFirewall->keySequence().toString());
        }
    });

    connect(ui.tbClearStopFirewallHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStopFirewall->clear();
        globalData->setFirewallStopHotkey("");
    });

    // 音效
    ui.leFirewallStartSoundPath->setText(globalData->firewallStartSound());
    connect(ui.tbFirewallSelectStartSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile(globalData->firewallStartSound());
        if (!fileName.isEmpty()) {
            ui.leFirewallStartSoundPath->setText(fileName);
            globalData->setFirewallStartSound(fileName);
        }
    });
    connect(ui.leFirewallStartSoundPath, &QLineEdit::textChanged, this, [=](const QString& text) {
        globalData->setFirewallStartSound(text);
    });
    connect(ui.tbFirewallPlayStartSound, &QAbstractButton::clicked, this, [=]() {
        PlaySound(globalData->firewallStartSound().toStdWString().c_str(),
            nullptr, SND_FILENAME | SND_ASYNC);
    });

    ui.leFirewallStopSoundPath->setText(globalData->firewallStopSound());
    connect(ui.tbFirewallSelectStopSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile(globalData->firewallStopSound());
        if (!fileName.isEmpty()) {
            ui.leFirewallStopSoundPath->setText(fileName);
            globalData->setFirewallStopSound(fileName);
        }
    });
    connect(ui.leFirewallStopSoundPath, &QLineEdit::textChanged, this, [=](const QString& text) {
        globalData->setFirewallStopSound(text);
    });
    connect(ui.tbFirewallPlayStopSound, &QAbstractButton::clicked, this, [=]() {
        PlaySound(globalData->firewallStopSound().toStdWString().c_str(),
            nullptr, SND_FILENAME | SND_ASYNC);
    });

    ui.leFirewallErrorSoundPath->setText(globalData->firewallErrorSound());
    connect(ui.tbFirewallSelectErrorSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile(globalData->firewallErrorSound());
        if (!fileName.isEmpty()) {
            ui.leFirewallErrorSoundPath->setText(fileName);
            globalData->setFirewallErrorSound(fileName);
        }
    });
    connect(ui.leFirewallErrorSoundPath, &QLineEdit::textChanged, this, [=](const QString& text) {
        globalData->setFirewallErrorSound(text);
    });
    connect(ui.tbFirewallPlayErrorSound, &QAbstractButton::clicked, this, [=]() {
        PlaySound(globalData->firewallErrorSound().toStdWString().c_str(),
            nullptr, SND_FILENAME | SND_ASYNC);
    });

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
}

// 任务信息设置
void SettingDialog::initMissionDataSettings()
{
    // 刷新间隔
    ui.sbMissionDataUpdateInterval->setValue(globalData->missionDataUpdateInterval());
    connect(ui.sbMissionDataUpdateInterval, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setMissionDataUpdateInterval(value);
    });

    // 数据名称显示方式 ===========================================
    {
        int i = 0;
        int currentMissionDataNameIndex = 0;
        for (auto s : globalData->missionDataNames()) {
            ui.cbMissionDataName->addItem(MissionDataNameUtil::toDisplayString(s), s);
            if (s == globalData->missionDataName()) {
                currentMissionDataNameIndex = i;
            }
            i++;
        }
        connect(ui.cbMissionDataName, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                globalData->setMissionDataName(
                    ui.cbMissionDataName->itemData(index).value<MissionDataNameUtil::MissionDataName>());
            });
        ui.cbMissionDataName->setCurrentIndex(currentMissionDataNameIndex);
    }

    // 数据组合 ===========================================
    connect(ui.cbMissionDataCombine, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            const auto includedDataFetchers = missionStrategyUtil->missionToDataFetchers()
                                                  [ui.cbMissionDataCombine->itemData(index).value<QString>()];
            setMissionDataCombineListWidgetData(includedDataFetchers);
        });

    for (auto mission : missionStrategyUtil->missionStrategies) {
        ui.cbMissionDataCombine->addItem(mission->getUniqueDisplayName(), mission->id());
    }

    // 有选中时按钮才可用
    ui.tbMissionDataCombineAdd->setEnabled(false);
    connect(ui.lwMissionDataCombineExcluded, &QListWidget::currentRowChanged, this, [=](int currentRow) {
        ui.tbMissionDataCombineAdd->setEnabled(currentRow >= 0);
    });
    ui.tbMissionDataCombineRemove->setEnabled(false);
    connect(ui.lwMissionDataCombineIncluded, &QListWidget::currentRowChanged, this, [=](int currentRow) {
        ui.tbMissionDataCombineRemove->setEnabled(currentRow >= 0);
    });

    connect(ui.tbMissionDataCombineAdd, &QAbstractButton::clicked, this, [=]() {
        auto currentExcludedDataFetcher = ui.lwMissionDataCombineExcluded
                                              ->takeItem(ui.lwMissionDataCombineExcluded->currentRow());
        ui.lwMissionDataCombineIncluded->addItem(currentExcludedDataFetcher);
        QString currentMissionId = ui.cbMissionDataCombine->currentData().value<QString>();
        QString currentExcludedId = currentExcludedDataFetcher->data(Qt::UserRole).value<QString>();
        missionStrategyUtil->insertToMissionToDataFetchers(currentMissionId, currentExcludedId);
    });
    connect(ui.tbMissionDataCombineRemove, &QAbstractButton::clicked, this, [=]() {
        auto currentIncludedDataFetcher = ui.lwMissionDataCombineIncluded
                                              ->takeItem(ui.lwMissionDataCombineIncluded->currentRow());
        ui.lwMissionDataCombineExcluded->addItem(currentIncludedDataFetcher);
        QString currentMissionId = ui.cbMissionDataCombine->currentData().value<QString>();
        QString currentIncludedId = currentIncludedDataFetcher->data(Qt::UserRole).value<QString>();
        missionStrategyUtil->removeFromMissionToDataFetchers(currentMissionId, currentIncludedId);
    });
    connect(ui.pbMissionDataCombineReset, &QAbstractButton::clicked, this, [=]() {
        QString currentMissionId = ui.cbMissionDataCombine->currentData().value<QString>();
        missionStrategyUtil->updateMissionToDataFetchers(currentMissionId,
            missionStrategyUtil->missionIdToObject[currentMissionId]->defaultDataFetchers());

        setMissionDataCombineListWidgetData(missionStrategyUtil->missionToDataFetchers()[currentMissionId]);
    });
}

void SettingDialog::setMissionDataCombineListWidgetData(QSet<QString> includedDataFetchers)
{
    // Included
    ui.lwMissionDataCombineIncluded->clear();
    for (const QString& dataFetcherId : includedDataFetchers) {
        BaseDataFetcher* currentDataFetcher = dataFetcherUtil->dataFetcherMap[dataFetcherId];
        QListWidgetItem* newItem = new QListWidgetItem(currentDataFetcher->getDisplayName(), ui.lwMissionDataCombineIncluded);
        newItem->setData(Qt::UserRole, dataFetcherId);
        ui.lwMissionDataCombineIncluded->addItem(newItem);
    }
    // Excluded
    ui.lwMissionDataCombineExcluded->clear();
    QList<QString> excludedDataFetchers = {};
    for (auto fetcher : dataFetcherUtil->dataFetchers) {
        if (!includedDataFetchers.contains(fetcher->id())) {
            excludedDataFetchers.append(fetcher->id());
        }
    }
    for (QString& dataFetcherId : excludedDataFetchers) {
        BaseDataFetcher* currentDataFetcher = dataFetcherUtil->dataFetcherMap[dataFetcherId];
        QListWidgetItem* newItem = new QListWidgetItem(currentDataFetcher->getDisplayName(), ui.lwMissionDataCombineExcluded);
        newItem->setData(Qt::UserRole, dataFetcherId);
        ui.lwMissionDataCombineExcluded->addItem(newItem);
    }
}

// 计时器设置
void SettingDialog::initTimerSettings()
{
    ui.keySeqStartTimer->setKeySequence(QKeySequence(globalData->timerStartHotkey()));
    connect(ui.keySeqStartTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStartTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStartTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStartTimer->setKeySequence(shortcut);
            globalData->setTimerStartHotkey(shortcut.toString());
        } else {
            globalData->setTimerStartHotkey(ui.keySeqStartTimer->keySequence().toString());
        }
    });

    connect(ui.tbClearStartTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStartTimer->clear();
        globalData->setTimerStartHotkey("");
    });

    ui.keySeqPauseTimer->setKeySequence(QKeySequence(globalData->timerPauseHotkey()));
    connect(ui.keySeqPauseTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqPauseTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqPauseTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqPauseTimer->setKeySequence(shortcut);
            globalData->setTimerPauseHotkey(shortcut.toString());
        } else {
            globalData->setTimerPauseHotkey(ui.keySeqPauseTimer->keySequence().toString());
        }
    });

    connect(ui.tbClearPauseTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqPauseTimer->clear();
        globalData->setTimerPauseHotkey("");
    });

    ui.keySeqStopTimer->setKeySequence(QKeySequence(globalData->timerStopHotkey()));
    connect(ui.keySeqStopTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStopTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStopTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStopTimer->setKeySequence(shortcut);
            globalData->setTimerStopHotkey(shortcut.toString());
        } else {
            globalData->setTimerStopHotkey(ui.keySeqStopTimer->keySequence().toString());
        }
    });

    connect(ui.tbClearStopTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStopTimer->clear();
        globalData->setTimerStopHotkey("");
    });

    // 手动计时刷新间隔
    ui.sbTimerUpdateInterval->setValue(globalData->timerUpdateInterval());
    connect(ui.sbTimerUpdateInterval, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setTimerUpdateInterval(value);
    });
    // 自动计时刷新间隔
    ui.sbAutoTimerUpdateInterval->setValue(globalData->autoTimerUpdateInterval());
    connect(ui.sbAutoTimerUpdateInterval, &QSpinBox::valueChanged, this, [=](int value) {
        globalData->setAutoTimerUpdateInterval(value);
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
    connect(ui.cbDisplayInfoToolWindow, &QCheckBox::stateChanged, this, [=](int state) {
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
    connect(ui.cbDisplayInfoFuncEnable, &QCheckBox::stateChanged, this, [=](int state) {
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
            setDisplayInfoCententSettings(currentSubFunction);
        });
    setDisplayInfoCententSettings(currentSubFunction);
    ui.cbDisplayInfoFunction->setCurrentIndex(currentSubFunctionIndex);
}

void SettingDialog::initCloseGameImmediatelySettings()
{
    ui.keySeqCloseGameImmediately->setKeySequence(QKeySequence(globalData->closeGameImmediatelyHotkey()));
    connect(ui.keySeqCloseGameImmediately, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqCloseGameImmediately->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqCloseGameImmediately->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqCloseGameImmediately->setKeySequence(shortcut);
            globalData->setCloseGameImmediatelyHotkey(shortcut.toString());
        } else {
            globalData->setCloseGameImmediatelyHotkey(ui.keySeqCloseGameImmediately->keySequence().toString());
        }
    });

    connect(ui.tbClearCloseGameImmediatelyHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqCloseGameImmediately->clear();
        globalData->setCloseGameImmediatelyHotkey("");
    });
}

void SettingDialog::initSocialSettings()
{
    ui.cbDiscordShowRp->setChecked(globalData->discordShowRichPresence());
    connect(ui.cbDiscordShowRp, &QCheckBox::stateChanged, this, [=](int state) {
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
    connect(ui.cbDebugMode, &QCheckBox::stateChanged, this, [=](int state) {
        globalData->setDebugMode(state == Qt::Checked);
    });
}

void SettingDialog::setDisplayInfoCententSettings(DisplayInfoSubFunction f)
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
