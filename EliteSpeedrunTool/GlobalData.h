#pragma once

#include "DisplayInfoSubFunction.h"
#include <QColor>
#include <QMetaType>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QString>
#include <icftypes.h>

#define globalData (GlobalData::instance())

typedef QMap<DisplayInfoSubFunction, DisplayInfoSubFunctionItem*> QDisplayInfoSubFuncsMap;

enum TimerStopStrategy {
    OnlyStop,
    StopAndZero,
    StopSecondZero
};

namespace TimerStopStrategyUtil {
TimerStopStrategy fromString(QString strategy);
QString toDisplayString(TimerStopStrategy strategy);
QString toString(TimerStopStrategy strategy);
}

namespace MissionDataNameUtil {
enum MissionDataName {
    FullName,
    Emoji,
    None
};
MissionDataName fromString(QString name);
QString toDisplayString(MissionDataName name);
QString toString(MissionDataName name);
}

class GlobalData : public QObject {
    Q_OBJECT

public:
    GlobalData();

    static GlobalData* instance();

    inline QString getSettingsFilePath();

    void init();
    void destory();

    void readSettings();

    void writeSettings();

private:
    // 读取/写入信息展示设置
    void readSubFuncSettingsMap(QSettings& settings);
    void writeSubFuncSettingsMap(QSettings& settings);

    // =====================================================
public:
    bool minimizeToTray() const;
    void setMinimizeToTray(bool newMinimizeToTray);

    const QString& styleName() const;
    void setStyleName(const QString& newStyleName);

    bool autoCheckUpdate() const;
    void setAutoCheckUpdate(bool newAutoCheckUpdate);

    const QString& version() const;
    void setVersion(const QString& newVersion);

    const QString& ignoredNewVersion() const;
    void setIgnoredNewVersion(const QString& newIgnoredNewVersion);

    const QList<DisplayInfoSubFunction>& funcs() const;

    const QDisplayInfoSubFuncsMap& displayInfoSubFunctions() const;

    bool displayInfoShow() const;
    void setDisplayInfoShow(bool displayInfoShow);

    bool displayInfoTouchable() const;
    void setDisplayInfoTouchable(bool newDisplayInfoTouchable);

    bool displayInfoServer() const;
    void setDisplayInfoServer(bool newDisplayInfoServer);

    QPoint displayInfoPos() const;
    void setDisplayInfoPos(QPoint newDisplayInfoPos);

    const QSize& displayInfoSize() const;
    void setDisplayInfoSize(const QSize& newDisplayInfoSize);

    const QColor& displayInfoBackground() const;
    void setDisplayInfoBackground(const QColor& newDisplayInfoBackground);

    const QString& firewallStartHotkey() const;
    void setFirewallStartHotkey(const QString& newFirewallStartHotkey);

    const QString& firewallStopHotkey() const;
    void setFirewallStopHotkey(const QString& newFirewallStopHotkey);

    const QString& firewallStartSound() const;
    void setFirewallStartSound(const QString& newFirewallStartSound);

    const QString& firewallStopSound() const;
    void setFirewallStopSound(const QString& newFirewallStopSound);

    const QString& firewallErrorSound() const;
    void setFirewallErrorSound(const QString& newFirewallErrorSound);

    bool firewallPlaySound() const;
    void setFirewallPlaySound(bool newFirewallPlaySound);

    const QString& firewallAppPath() const;
    void setFirewallAppPath(const QString& newFirewallAppPath);

    int firewallDirection() const;
    void setFirewallDirection(int newFirewallDirection);

    int missionDataUpdateInterval() const;
    void setMissionDataUpdateInterval(int newMissionDataUpdateInterval);

    const QString& timerStartHotkey() const;
    void setTimerStartHotkey(const QString& newTimerStartHotkey);

    const QString& timerPauseHotkey() const;
    void setTimerPauseHotkey(const QString& newTimerPauseHotkey);

    const QString& timerStopHotkey() const;
    void setTimerStopHotkey(const QString& newTimerStopHotkey);

    const QList<TimerStopStrategy>& timerStopStrategies() const;

    TimerStopStrategy timerStopStrategy() const;
    void setTimerStopStrategy(TimerStopStrategy newTimerStopStrategy);

    int timerUpdateInterval() const;
    void setTimerUpdateInterval(int newTimerUpdateInterval);

    const QString& language() const;
    void setLanguage(const QString& newLanguage);

    int serverHttpPort() const;
    void setServerHttpPort(int newServerHttpPort);

    int serverWebsocketPort() const;
    void setServerWebsocketPort(int newServerWebsocketPort);

    bool discordShowRichPresence() const;
    void setDiscordShowRichPresence(bool newDiscordShowRichPresence);

    const QString& closeGameImmediatelyHotkey() const;
    void setCloseGameImmediatelyHotkey(const QString& newCloseGameImmediatelyHotkey);

    int autoTimerUpdateInterval() const;
    void setAutoTimerUpdateInterval(int newAutoTimerUpdateInterval);

    QList<MissionDataNameUtil::MissionDataName> missionDataNames() const;

    MissionDataNameUtil::MissionDataName missionDataName() const;
    void setMissionDataName(MissionDataNameUtil::MissionDataName newMissionDataName);

    bool debugMode() const;
    void setDebugMode(bool newDebugMode);

signals:
    void minimizeToTrayChanged();
    void styleNameChanged();

    void autoCheckUpdateChanged();
    void versionChanged();
    void ignoredNewVersionChanged();

    void displayInfoShowChanged();
    void displayInfoTouchableChanged();
    void displayInfoServerChanged();
    void displayInfoPosChanged();
    void displayInfoSizeChanged();
    void displayInfoBackgroundChanged();

    void firewallStartHotkeyChanged();
    void firewallStopHotkeyChanged();
    void firewallStartSoundChanged();
    void firewallStopSoundChanged();
    void firewallErrorSoundChanged();
    void firewallPlaySoundChanged();
    void firewallAppPathChanged();
    void firewallDirectionChanged();

    void missionDataUpdateIntervalChanged();

    void timerStartHotkeyChanged();
    void timerPauseHotkeyChanged();
    void timerStopHotkeyChanged();
    void timerStopStrategyChanged();
    void timerUpdateIntervalChanged();
    void autoTimerUpdateIntervalChanged();

    void languageChanged();

    void serverHttpPortChanged();
    void serverWebsocketPortChanged();

    void discordShowRichPresenceChanged();

    void closeGameImmediatelyHotkeyChanged();

    void missionDataNameChanged();

    void debugModeChanged();

private:
    bool mMinimizeToTray = false;
    QString mStyleName = "windowsvista";
    // 版本
    bool mAutoCheckUpdate = true;
    QString mVersion = "0.0";
    QString mIgnoredNewVersion = "";

    // 信息展示
    QList<DisplayInfoSubFunction> mFuncs = {
        DisplayInfoSubFunction::Firewall,
        DisplayInfoSubFunction::Headshot,
        DisplayInfoSubFunction::Timer,
        DisplayInfoSubFunction::AutoTimer,
        DisplayInfoSubFunction::VehicleDamage,
        DisplayInfoSubFunction::RashkovskyDamage,
        DisplayInfoSubFunction::Kill,
        DisplayInfoSubFunction::PlaneDamage,
    };
    QDisplayInfoSubFuncsMap mDisplayInfoSubFunctions;
    bool mDisplayInfoShow = true;
    bool mDisplayInfoTouchable = true;
    bool mDisplayInfoServer = false;
    QPoint mDisplayInfoPos = { 20, 20 };
    QSize mDisplayInfoSize = { 300, 100 };
    QColor mDisplayInfoBackground = Qt::transparent;

    // 防火墙
    QString mFirewallStartHotkey = "F9";
    QString mFirewallStopHotkey = "F9";
    QString mFirewallStartSound = "./sound/chimes.wav";
    QString mFirewallStopSound = "./sound/ding.wav";
    QString mFirewallErrorSound = "./sound/error.wav";
    bool mFirewallPlaySound = true;
    QString mFirewallAppPath = "";
    int mFirewallDirection = NET_FW_RULE_DIR_OUT;

    // 任务数据
    int mMissionDataUpdateInterval = 100;
    QList<MissionDataNameUtil::MissionDataName> mMissionDataNames = {
        MissionDataNameUtil::MissionDataName::FullName,
        MissionDataNameUtil::MissionDataName::Emoji,
        MissionDataNameUtil::MissionDataName::None
    };
    MissionDataNameUtil::MissionDataName mMissionDataName = MissionDataNameUtil::MissionDataName::FullName;

    // 计时器
    QString mTimerStartHotkey = "F7";
    QString mTimerPauseHotkey = "F8";
    QString mTimerStopHotkey = "F7";
    QList<TimerStopStrategy> mTimerStopStrategies = {
        TimerStopStrategy::OnlyStop,
        TimerStopStrategy::StopAndZero,
        TimerStopStrategy::StopSecondZero
    };
    TimerStopStrategy mTimerStopStrategy = TimerStopStrategy::OnlyStop;
    int mTimerUpdateInterval = 50;
    int mAutoTimerUpdateInterval = 50;

    // 语言
    QString mLanguage = "";

    // 服务器
    int mServerHttpPort = 9975;
    int mServerWebsocketPort = 9976;

    // 社交
    bool mDiscordShowRichPresence = false;

    // 卡前置
    QString mCloseGameImmediatelyHotkey = "Ctrl+`";

    // 开发者选项
    bool mDebugMode = false;

private:
    Q_PROPERTY(bool minimizeToTray READ minimizeToTray WRITE setMinimizeToTray NOTIFY minimizeToTrayChanged)
    Q_PROPERTY(QString styleName READ styleName WRITE setStyleName NOTIFY styleNameChanged)
    Q_PROPERTY(bool autoCheckUpdate READ autoCheckUpdate WRITE setAutoCheckUpdate NOTIFY autoCheckUpdateChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString ignoredNewVersion READ ignoredNewVersion WRITE setIgnoredNewVersion NOTIFY ignoredNewVersionChanged)
    Q_PROPERTY(QList<DisplayInfoSubFunction> funcs READ funcs CONSTANT)
    Q_PROPERTY(QDisplayInfoSubFuncsMap displayInfoSubFunctions READ displayInfoSubFunctions CONSTANT)
    Q_PROPERTY(bool displayInfoShow READ displayInfoShow WRITE setDisplayInfoShow NOTIFY displayInfoShowChanged)
    Q_PROPERTY(bool displayInfoTouchable READ displayInfoTouchable WRITE setDisplayInfoTouchable NOTIFY displayInfoTouchableChanged)
    Q_PROPERTY(bool displayInfoServer READ displayInfoServer WRITE setDisplayInfoServer NOTIFY displayInfoServerChanged)
    Q_PROPERTY(QPoint displayInfoPos READ displayInfoPos WRITE setDisplayInfoPos NOTIFY displayInfoPosChanged)
    Q_PROPERTY(QSize displayInfoSize READ displayInfoSize WRITE setDisplayInfoSize NOTIFY displayInfoSizeChanged)
    Q_PROPERTY(QColor displayInfoBackground READ displayInfoBackground WRITE setDisplayInfoBackground NOTIFY displayInfoBackgroundChanged)
    Q_PROPERTY(QString firewallStartHotkey READ firewallStartHotkey WRITE setFirewallStartHotkey NOTIFY firewallStartHotkeyChanged)
    Q_PROPERTY(QString firewallStopHotkey READ firewallStopHotkey WRITE setFirewallStopHotkey NOTIFY firewallStopHotkeyChanged)
    Q_PROPERTY(QString firewallStartSound READ firewallStartSound WRITE setFirewallStartSound NOTIFY firewallStartSoundChanged)
    Q_PROPERTY(QString firewallStopSound READ firewallStopSound WRITE setFirewallStopSound NOTIFY firewallStopSoundChanged)
    Q_PROPERTY(QString firewallErrorSound READ firewallErrorSound WRITE setFirewallErrorSound NOTIFY firewallErrorSoundChanged)
    Q_PROPERTY(bool firewallPlaySound READ firewallPlaySound WRITE setFirewallPlaySound NOTIFY firewallPlaySoundChanged)
    Q_PROPERTY(QString firewallAppPath READ firewallAppPath WRITE setFirewallAppPath NOTIFY firewallAppPathChanged)
    Q_PROPERTY(int firewallDirection READ firewallDirection WRITE setFirewallDirection NOTIFY firewallDirectionChanged)
    Q_PROPERTY(int missionDataUpdateInterval READ missionDataUpdateInterval WRITE setMissionDataUpdateInterval NOTIFY missionDataUpdateIntervalChanged)
    Q_PROPERTY(QString timerStartHotkey READ timerStartHotkey WRITE setTimerStartHotkey NOTIFY timerStartHotkeyChanged)
    Q_PROPERTY(QString timerPauseHotkey READ timerPauseHotkey WRITE setTimerPauseHotkey NOTIFY timerPauseHotkeyChanged)
    Q_PROPERTY(QString timerStopHotkey READ timerStopHotkey WRITE setTimerStopHotkey NOTIFY timerStopHotkeyChanged)
    Q_PROPERTY(QList<TimerStopStrategy> timerStopStrategies READ timerStopStrategies CONSTANT)
    Q_PROPERTY(TimerStopStrategy timerStopStrategy READ timerStopStrategy WRITE setTimerStopStrategy NOTIFY timerStopStrategyChanged)
    Q_PROPERTY(int timerUpdateInterval READ timerUpdateInterval WRITE setTimerUpdateInterval NOTIFY timerUpdateIntervalChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(int serverHttpPort READ serverHttpPort WRITE setServerHttpPort NOTIFY serverHttpPortChanged)
    Q_PROPERTY(int serverWebsocketPort READ serverWebsocketPort WRITE setServerWebsocketPort NOTIFY serverWebsocketPortChanged)
    Q_PROPERTY(bool discordShowRichPresence READ discordShowRichPresence WRITE setDiscordShowRichPresence NOTIFY discordShowRichPresenceChanged)
    Q_PROPERTY(QString closeGameImmediatelyHotkey READ closeGameImmediatelyHotkey WRITE setCloseGameImmediatelyHotkey NOTIFY closeGameImmediatelyHotkeyChanged)
    Q_PROPERTY(int autoTimerUpdateInterval READ autoTimerUpdateInterval WRITE setAutoTimerUpdateInterval NOTIFY autoTimerUpdateIntervalChanged)
    Q_PROPERTY(MissionDataNameUtil::MissionDataName missionDataName READ missionDataName WRITE setMissionDataName NOTIFY missionDataNameChanged FINAL)
    Q_PROPERTY(bool debugMode READ debugMode WRITE setDebugMode NOTIFY debugModeChanged FINAL)
};
