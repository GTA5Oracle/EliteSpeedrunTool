#pragma once

#include "displayinfo/DisplayInfoSubFunction.h"
#include <QColor>
#include <QMetaType>
#include <QPoint>
#include <QRegion>
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

    QString getSettingsFilePath();

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

    bool displayInfoToolWindow() const;
    void setDisplayInfoToolWindow(bool newDisplayInfoToolWindow);

    QString suspendAndResumeHotkey() const;
    void setSuspendAndResumeHotkey(const QString& newSuspendAndResumeHotkey);

    int suspendAndResumeDuration() const;
    void setSuspendAndResumeDuration(int newSuspendAndResumeDuration);

    QRect rpRect() const;
    void setRpRect(const QRect& newRpRegion);

    QString act3HeadshotStartHotkey() const;
    void setAct3HeadshotStartHotkey(const QString& newAct3HeadshotStartHotkey);

    QString act3HeadshotStopHotkey() const;
    void setAct3HeadshotStopHotkey(const QString& newAct3HeadshotStopHotkey);

    QString act3HeadshotStartSound() const;
    void setAct3HeadshotStartSound(const QString& newAct3HeadshotStartSound);

    QString act3HeadshotStopSound() const;
    void setAct3HeadshotStopSound(const QString& newAct3HeadshotStopSound);

    bool rtssOverlay() const;
    void setRtssOverlay(bool newRtssOverlay);

    QList<QString> selectedNetworkAdapters() const;
    void setSelectedNetworkAdapters(const QList<QString>& newSelectedNetworkAdapters);

    QString networkAdaptersEnable() const;
    void setNetworkAdaptersEnable(const QString& newNetworkAdaptersEnable);

    QString networkAdaptersDisable() const;
    void setNetworkAdaptersDisable(const QString& newNetworkAdaptersDisable);

    QString networkAdaptersEnableHotkey() const;
    void setNetworkAdaptersEnableHotkey(const QString& newNetworkAdaptersEnableHotkey);

    QString networkAdaptersDisableHotkey() const;
    void setNetworkAdaptersDisableHotkey(const QString& newNetworkAdaptersDisableHotkey);

    QString networkAdaptersEnableSound() const;
    void setNetworkAdaptersEnableSound(const QString& newNetworkAdaptersEnableSound);

    QString networkAdaptersDisableSound() const;
    void setNetworkAdaptersDisableSound(const QString& newNetworkAdaptersDisableSound);

    bool rtssCrosshair() const;
    void setRtssCrosshair(bool newRtssCrosshair);

    QString rtssCrosshairText() const;
    void setRtssCrosshairText(const QString& newRtssCrosshairText);

    int rtssCrosshairX() const;
    void setRtssCrosshairX(int newRtssCrosshairX);

    int rtssCrosshairY() const;
    void setRtssCrosshairY(int newRtssCrosshairY);

    int rtssCrosshairSize() const;
    void setRtssCrosshairSize(int newRtssCrosshairSize);

    QColor rtssCrosshairColor() const;
    void setRtssCrosshairColor(QColor newRtssCrosshairColor);

    QString topMostWindowHotkey() const;
    void setTopMostWindowHotkey(const QString& newShowToolHotkey);

    int firewallProtocol() const;
    void setFirewallProtocol(int newFirewallProtocol);

    QList<QString> hotkeyMaps() const;
    void setHotkeyMaps(const QList<QString>& newHotkeyMaps);

    QMap<QString, QString>& eventCmdMap();
    void setEventCmdMap(const QMap<QString, QString>& newEventCmdMap);

    QPoint crosshairOffset() const;
    void setCrosshairOffset(QPoint newCrosshairOffset);

    QSize crosshairSize() const;
    void setCrosshairSize(const QSize& newCrosshairSize);

    QString crosshairImage() const;
    void setCrosshairImage(const QString& newCrosshairImage);

    bool crosshairShow() const;
    void setCrosshairShow(bool newCrosshairShow);

    QColor crosshairShadowColor() const;
    void setCrosshairShadowColor(const QColor &newCrosshairShadowColor);

    int crosshairShadowBlurRadius() const;
    void setCrosshairShadowBlurRadius(int newCrosshairShadowBlurRadius);

    QPointF crosshairShadowOffset() const;
    void setCrosshairShadowOffset(QPointF newCrosshairShadowOffset);

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

    void displayInfoToolWindowChanged();

    void suspendAndResumeHotkeyChanged();
    void suspendAndResumeDurationChanged();

    void rpRectChanged();

    void act3HeadshotStartHotkeyChanged();
    void act3HeadshotStopHotkeyChanged();
    void act3HeadshotStartSoundChanged();
    void act3HeadshotStopSoundChanged();

    void rtssOverlayChanged();

    void selectedNetworkAdaptersChanged();

    void networkAdaptersEnableHotkeyChanged();

    void networkAdaptersDisableHotkeyChanged();

    void networkAdaptersEnableSoundChanged();

    void networkAdaptersDisableSoundChanged();

    void rtssCrosshairChanged();

    void rtssCrosshairTextChanged();

    void rtssCrosshairXChanged();

    void rtssCrosshairYChanged();

    void rtssCrosshairSizeChanged();

    void rtssCrosshairColorChanged();

    void topMostWindowHotkeyChanged();

    void firewallProtocolChanged();

    void hotkeyMapsChanged();

    void eventCmdMapChanged();

    void crosshairOffsetChanged();

    void crosshairSizeChanged();

    void crosshairImageChanged();

    void crosshairShowChanged();

    void crosshairShadowColorChanged();

    void crosshairShadowBlurRadiusChanged();

    void crosshairShadowOffsetChanged();

private:
    bool mMinimizeToTray = false;
    QString mTopMostWindowHotkey = "";
    QString mStyleName = "windows11";
    // 版本
    bool mAutoCheckUpdate = true;
    QString mVersion = "0.0";
    QString mIgnoredNewVersion = "";

    // 信息展示
    QList<DisplayInfoSubFunction> mFuncs = {
        DisplayInfoSubFunction::Firewall,
        DisplayInfoSubFunction::Timer,
        DisplayInfoSubFunction::Act3Headshot,
    };
    QDisplayInfoSubFuncsMap mDisplayInfoSubFunctions;
    bool mDisplayInfoToolWindow = true;
    bool mDisplayInfoShow = true;
    bool mDisplayInfoTouchable = true;
    bool mDisplayInfoServer = false;
    QPoint mDisplayInfoPos = { 20, 20 };
    QSize mDisplayInfoSize = { 300, 100 };
    QColor mDisplayInfoBackground = Qt::transparent;

    // Crosshair
    bool mCrosshairShow = true;
    QString mCrosshairImage = "";
    QPoint mCrosshairOffset = { 0, 0 };
    QSize mCrosshairSize = { 4, 4 };
    QColor mCrosshairShadowColor = Qt::black;
    int mCrosshairShadowBlurRadius = 7;
    QPointF mCrosshairShadowOffset = { 0, 0 };

    // RTSS
    bool mRtssOverlay = true;
    bool mRtssCrosshair = false;
    QString mRtssCrosshairText = "*";
    int mRtssCrosshairX = 200;
    int mRtssCrosshairY = 200;
    int mRtssCrosshairSize = 80;
    QColor mRtssCrosshairColor = Qt::yellow;

    // 防火墙
    QString mFirewallStartHotkey = "F9";
    QString mFirewallStopHotkey = "F9";
    QString mFirewallStartSound = "./sound/chimes.wav";
    QString mFirewallStopSound = "./sound/ding.wav";
    QString mFirewallErrorSound = "./sound/error.wav";
    bool mFirewallPlaySound = true;
    QString mFirewallAppPath = "";
    int mFirewallDirection = NET_FW_RULE_DIR_OUT;
    int mFirewallProtocol = -1;

    // 网络适配器
    QList<QString> mSelectedNetworkAdapters = {};
    QString mNetworkAdaptersEnableHotkey = "Alt+Q";
    QString mNetworkAdaptersDisableHotkey = "Alt+Q";
    QString mNetworkAdaptersEnableSound = "./sound/uradaron.wav";
    QString mNetworkAdaptersDisableSound = "./sound/uradarof.wav";

    // 任务数据
    int mMissionDataUpdateInterval = 50;
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
    int mTimerUpdateInterval = 30;
    int mAutoTimerUpdateInterval = 30;

    // 语言
    QString mLanguage = "";

    // 服务器
    int mServerHttpPort = 9975;
    int mServerWebsocketPort = 9976;

    // 社交
    bool mDiscordShowRichPresence = false;

    // 卡单
    QString mSuspendAndResumeHotkey = "Ctrl+Alt+`";
    int mSuspendAndResumeDuration = 9000;

    // 末日将至爆头识别
    QString mAct3HeadshotStartHotkey = "F2";
    QString mAct3HeadshotStopHotkey = "F3";
    QString mAct3HeadshotStartSound = "./sound/genter1a.wav";
    QString mAct3HeadshotStopSound = "./sound/uplan1.wav";

    // 结束游戏
    QString mCloseGameImmediatelyHotkey = "Ctrl+`";

    // 快捷键映射
    QList<QString> mHotkeyMaps = {};

    // 事件命令
    QMap<QString, QString> mEventCmdMap = {};

    // 开发者选项
    bool mDebugMode = false;

    // 经验条
    QRect mRpRect = QRect(0, 0, 0, 0);

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
    Q_PROPERTY(bool displayInfoToolWindow READ displayInfoToolWindow WRITE setDisplayInfoToolWindow NOTIFY displayInfoToolWindowChanged FINAL)
    Q_PROPERTY(QString suspendAndResumeHotkey READ suspendAndResumeHotkey WRITE setSuspendAndResumeHotkey NOTIFY suspendAndResumeHotkeyChanged FINAL)
    Q_PROPERTY(int suspendAndResumeDuration READ suspendAndResumeDuration WRITE setSuspendAndResumeDuration NOTIFY suspendAndResumeDurationChanged FINAL)
    Q_PROPERTY(QRect rpRect READ rpRect WRITE setRpRect NOTIFY rpRectChanged FINAL)
    Q_PROPERTY(QString act3HeadshotStartHotkey READ act3HeadshotStartHotkey WRITE setAct3HeadshotStartHotkey NOTIFY act3HeadshotStartHotkeyChanged FINAL)
    Q_PROPERTY(QString act3HeadshotStopHotkey READ act3HeadshotStopHotkey WRITE setAct3HeadshotStopHotkey NOTIFY act3HeadshotStopHotkeyChanged FINAL)
    Q_PROPERTY(QString act3HeadshotStartSound READ act3HeadshotStartSound WRITE setAct3HeadshotStartSound NOTIFY act3HeadshotStartSoundChanged FINAL)
    Q_PROPERTY(QString act3HeadshotStopSound READ act3HeadshotStopSound WRITE setAct3HeadshotStopSound NOTIFY act3HeadshotStopSoundChanged FINAL)
    Q_PROPERTY(bool rtssOverlay READ rtssOverlay WRITE setRtssOverlay NOTIFY rtssOverlayChanged FINAL)
    Q_PROPERTY(QList<QString> selectedNetworkAdapters READ selectedNetworkAdapters WRITE setSelectedNetworkAdapters NOTIFY selectedNetworkAdaptersChanged FINAL)
    Q_PROPERTY(QString networkAdaptersEnableHotkey READ networkAdaptersEnableHotkey WRITE setNetworkAdaptersEnableHotkey NOTIFY networkAdaptersEnableHotkeyChanged FINAL)
    Q_PROPERTY(QString networkAdaptersDisableHotkey READ networkAdaptersDisableHotkey WRITE setNetworkAdaptersDisableHotkey NOTIFY networkAdaptersDisableHotkeyChanged FINAL)
    Q_PROPERTY(QString networkAdaptersEnableSound READ networkAdaptersEnableSound WRITE setNetworkAdaptersEnableSound NOTIFY networkAdaptersEnableSoundChanged FINAL)
    Q_PROPERTY(QString networkAdaptersDisableSound READ networkAdaptersDisableSound WRITE setNetworkAdaptersDisableSound NOTIFY networkAdaptersDisableSoundChanged FINAL)
    Q_PROPERTY(bool rtssCrosshair READ rtssCrosshair WRITE setRtssCrosshair NOTIFY rtssCrosshairChanged FINAL)
    Q_PROPERTY(QString rtssCrosshairText READ rtssCrosshairText WRITE setRtssCrosshairText NOTIFY rtssCrosshairTextChanged FINAL)
    Q_PROPERTY(int rtssCrosshairX READ rtssCrosshairX WRITE setRtssCrosshairX NOTIFY rtssCrosshairXChanged FINAL)
    Q_PROPERTY(int rtssCrosshairY READ rtssCrosshairY WRITE setRtssCrosshairY NOTIFY rtssCrosshairYChanged FINAL)
    Q_PROPERTY(int rtssCrosshairSize READ rtssCrosshairSize WRITE setRtssCrosshairSize NOTIFY rtssCrosshairSizeChanged FINAL)
    Q_PROPERTY(QColor rtssCrosshairColor READ rtssCrosshairColor WRITE setRtssCrosshairColor NOTIFY rtssCrosshairColorChanged FINAL)
    Q_PROPERTY(QString topMostWindowHotkey READ topMostWindowHotkey WRITE setTopMostWindowHotkey NOTIFY topMostWindowHotkeyChanged FINAL)
    Q_PROPERTY(int firewallProtocol READ firewallProtocol WRITE setFirewallProtocol NOTIFY firewallProtocolChanged FINAL)
    Q_PROPERTY(QList<QString> hotkeyMaps READ hotkeyMaps WRITE setHotkeyMaps NOTIFY hotkeyMapsChanged FINAL)
    Q_PROPERTY(QMap<QString, QString> eventCmdMap READ eventCmdMap WRITE setEventCmdMap NOTIFY eventCmdMapChanged FINAL)
    Q_PROPERTY(QPoint crosshairOffset READ crosshairOffset WRITE setCrosshairOffset NOTIFY crosshairOffsetChanged FINAL)
    Q_PROPERTY(QSize crosshairSize READ crosshairSize WRITE setCrosshairSize NOTIFY crosshairSizeChanged FINAL)
    Q_PROPERTY(QString crosshairImage READ crosshairImage WRITE setCrosshairImage NOTIFY crosshairImageChanged FINAL)
    Q_PROPERTY(bool crosshairShow READ crosshairShow WRITE setCrosshairShow NOTIFY crosshairShowChanged FINAL)
    Q_PROPERTY(QColor crosshairShadowColor READ crosshairShadowColor WRITE setCrosshairShadowColor NOTIFY crosshairShadowColorChanged FINAL)
    Q_PROPERTY(int crosshairShadowBlurRadius READ crosshairShadowBlurRadius WRITE setCrosshairShadowBlurRadius NOTIFY crosshairShadowBlurRadiusChanged FINAL)
    Q_PROPERTY(QPointF crosshairShadowOffset READ crosshairShadowOffset WRITE setCrosshairShadowOffset NOTIFY crosshairShadowOffsetChanged FINAL)
};
