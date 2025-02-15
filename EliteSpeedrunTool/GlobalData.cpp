#include "GlobalData.h"
#include <QApplication>
#include <QColor>
#include <QFont>
#include <QSettings>
#include <windows.h>

Q_GLOBAL_STATIC(GlobalData, globalDataInstance)

namespace TimerStopStrategyUtil {
TimerStopStrategy fromString(QString strategy)
{
    if (strategy == "OnlyStop") {
        return TimerStopStrategy::OnlyStop;
    } else if (strategy == "StopAndZero") {
        return TimerStopStrategy::StopAndZero;
    } else if (strategy == "StopSecondZero") {
        return TimerStopStrategy::StopSecondZero;
    } else {
        return TimerStopStrategy::OnlyStop;
    }
}
QString toDisplayString(TimerStopStrategy strategy)
{
    switch (strategy) {
    case TimerStopStrategy::OnlyStop:
        return QObject::tr("停止后时间不归零");
        break;
    case TimerStopStrategy::StopAndZero:
        return QObject::tr("停止后时间立即归零");
        break;
    case TimerStopStrategy::StopSecondZero:
        return QObject::tr("停止后，再按一次停止键，时间才会归零");
        break;
    default:
        break;
    }
    return "";
}
QString toString(TimerStopStrategy strategy)
{
    switch (strategy) {
    case TimerStopStrategy::OnlyStop:
        return "OnlyStop";
        break;
    case TimerStopStrategy::StopAndZero:
        return "StopAndZero";
        break;
    case TimerStopStrategy::StopSecondZero:
        return "StopSecondZero";
        break;
    default:
        break;
    }
    return "OnlyStop";
}
}

namespace MissionDataNameUtil {
MissionDataName fromString(QString name)
{
    if (name == "FullName") {
        return MissionDataName::FullName;
    } else if (name == "Emoji") {
        return MissionDataName::Emoji;
    } else if (name == "None") {
        return MissionDataName::None;
    } else {
        return MissionDataName::FullName;
    }
}
QString toDisplayString(MissionDataName name)
{
    switch (name) {
    case MissionDataName::FullName:
        return QObject::tr("显示数据全名");
        break;
    case MissionDataName::Emoji:
        return QObject::tr("显示 Emoji");
        break;
    case MissionDataName::None:
        return QObject::tr("不显示数据名称");
        break;
    default:
        break;
    }
    return "";
}
QString toString(MissionDataName name)
{
    switch (name) {
    case MissionDataName::FullName:
        return "FullName";
        break;
    case MissionDataName::Emoji:
        return "Emoji";
        break;
    case MissionDataName::None:
        return "None";
        break;
    default:
        break;
    }
    return "FullName";
}
}

GlobalData::GlobalData()
{
}

GlobalData* GlobalData::instance()
{
    return globalDataInstance;
}

QString GlobalData::getSettingsFilePath()
{
    return QCoreApplication::applicationDirPath() + "/Config.ini";
}

void GlobalData::init()
{
    readSettings();
}

void GlobalData::destory()
{
    writeSettings();
}

void GlobalData::readSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("General");
    setStyleName(settings.value("StyleName", mStyleName).toString());
    setAutoCheckUpdate(settings.value("AutoCheckUpdate", mAutoCheckUpdate).toBool());
    setVersion(settings.value("Version", mVersion).toString());
    setIgnoredNewVersion(settings.value("IgnoredNewVersion", mIgnoredNewVersion).toString());
    setMinimizeToTray(settings.value("MinimizeToTray", mMinimizeToTray).toBool());
    setTopMostWindowHotkey(settings.value("TopMostWindowHotkey", mTopMostWindowHotkey).toString());
    setExcludeFromCapture(settings.value("ExcludeFromCapture", mExcludeFromCapture).toBool());
    settings.endGroup();

    settings.beginGroup("DisplayInfo");
    RECT rect;
    rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) + rect.left;
    rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) + rect.top;
    setDisplayInfoToolWindow(settings.value("DisplayInfoToolWindow", mDisplayInfoToolWindow).toBool());
    setDisplayInfoShow(settings.value("DisplayInfoShow", mDisplayInfoShow).toBool());
    setDisplayInfoTouchable(settings.value("DisplayInfoTouchable", mDisplayInfoShow).toBool());
    setDisplayInfoServer(settings.value("DisplayInfoServer", mDisplayInfoServer).toBool());
    setDisplayInfoSize(
        { qMax(qMin(settings.value("DisplayInfoWidth", mDisplayInfoSize.width()).toInt(), rect.right - rect.left), 10),
            qMax(qMin(settings.value("DisplayInfoHeight", mDisplayInfoSize.height()).toInt(), rect.bottom - rect.top), 10) });
    setDisplayInfoPos(
        { qMax(qMin(settings.value("DisplayInfoPosX", 20).toInt(), rect.right), rect.left),
            qMax(qMin(settings.value("DisplayInfoPosY", 20).toInt(), rect.bottom), rect.top) });
    setDisplayInfoBackground(QColor::fromRgba(settings.value("DisplayInfoBackground", mDisplayInfoBackground.rgba()).toUInt()));
    settings.endGroup();
    readSubFuncSettingsMap(settings);

    settings.beginGroup("Crosshair");
    setCrosshairShow(settings.value("CrosshairShow", mCrosshairShow).toBool());
    setCrosshairShowHotkey(settings.value("CrosshairShowHotkey", mCrosshairShowHotkey).toString());
    setCrosshairImage(settings.value("CrosshairImage", mCrosshairImage).toString());
    setCrosshairOffset(settings.value("CrosshairOffset", mCrosshairOffset).toPoint());
    setCrosshairSize(settings.value("CrosshairSize", mCrosshairSize).toSize());
    setCrosshairColor(settings.value("CrosshairColor", mCrosshairColor).value<QColor>());
    setCrosshairOpacity(settings.value("CrosshairOpacity", mCrosshairOpacity).toDouble());
    setCrosshairShadowColor(settings.value("CrosshairShadowColor", mCrosshairShadowColor).value<QColor>());
    setCrosshairShadowBlurRadius(settings.value("CrosshairShadowBlurRadius", mCrosshairShadowBlurRadius).toInt());
    setCrosshairShadowOffset(settings.value("CrosshairShadowOffset", mCrosshairShadowOffset).value<QPointF>());
    setCrosshairAttachWindowTitle(settings.value("CrosshairAttachWindowTitle", mCrosshairAttachWindowTitle).toString());
    settings.endGroup();

    settings.beginGroup("Rtss");
    setRtssOverlay(settings.value("RtssOverlay", mRtssOverlay).toBool());
    setRtssCrosshair(settings.value("RtssCrosshair", mRtssCrosshair).toBool());
    setRtssCrosshairText(settings.value("RtssCrosshairText", mRtssCrosshairText).toString());
    setRtssCrosshairX(settings.value("RtssCrosshairX", mRtssCrosshairX).toInt());
    setRtssCrosshairY(settings.value("RtssCrosshairY", mRtssCrosshairY).toInt());
    setRtssCrosshairSize(settings.value("RtssCrosshairSize", mRtssCrosshairSize).toInt());
    setRtssCrosshairColor(settings.value("RtssCrosshairColor", mRtssCrosshairColor).value<QColor>());
    settings.endGroup();

    settings.beginGroup("Firewall");
    setFirewallStartHotkey(settings.value("FirewallStartHotkey", mFirewallStartHotkey).toString());
    setFirewallStopHotkey(settings.value("FirewallStopHotkey", mFirewallStopHotkey).toString());
    setFirewallStartSound(settings.value("FirewallStartSound", mFirewallStartSound).toString());
    setFirewallStopSound(settings.value("FirewallStopSound", mFirewallStopSound).toString());
    setFirewallErrorSound(settings.value("FirewallErrorSound", mFirewallErrorSound).toString());
    setFirewallPlaySound(settings.value("FirewallPlaySound", mFirewallPlaySound).toBool());
    setFirewallAppPath(settings.value("FirewallAppPath", mFirewallAppPath).toString());
    setFirewallDirection(settings.value("FirewallDirection", mFirewallDirection).toInt());
    setFirewallProtocol(settings.value("FirewallProtocol", mFirewallProtocol).toInt());
    settings.endGroup();

    settings.beginGroup("NetworkAdapter");
    setSelectedNetworkAdapters(settings.value("SelectedNetworkAdapters", mSelectedNetworkAdapters).value<QList<QString>>());
    setNetworkAdaptersDisableHotkey(settings.value("NetworkAdaptersDisableHotkey", mNetworkAdaptersDisableHotkey).toString());
    setNetworkAdaptersEnableHotkey(settings.value("NetworkAdaptersEnableHotkey", mNetworkAdaptersEnableHotkey).toString());
    setNetworkAdaptersDisableSound(settings.value("NetworkAdaptersDisableSound", mNetworkAdaptersDisableSound).toString());
    setNetworkAdaptersEnableSound(settings.value("NetworkAdaptersEnableSound", mNetworkAdaptersEnableSound).toString());
    settings.endGroup();

    settings.beginGroup("MissionData");
    setMissionDataUpdateInterval(settings.value("MissionDataUpdateInterval", mMissionDataUpdateInterval).toInt());
    setMissionDataName(MissionDataNameUtil::fromString(
        settings.value("MissionDataName", MissionDataNameUtil::toString(mMissionDataName)).toString()));
    setRpRect(settings.value("RpRect", mRpRect).toRect());
    setAct3HeadshotStartHotkey(settings.value("Act3HeadshotStartHotkey", mAct3HeadshotStartHotkey).toString());
    setAct3HeadshotStopHotkey(settings.value("Act3HeadshotStopHotkey", mAct3HeadshotStopHotkey).toString());
    setAct3HeadshotStartSound(settings.value("Act3HeadshotStartSound", mAct3HeadshotStartSound).toString());
    setAct3HeadshotStopSound(settings.value("Act3HeadshotStopSound", mAct3HeadshotStopSound).toString());
    settings.endGroup();

    settings.beginGroup("Timer");
    setTimerStartHotkey(settings.value("TimerStartHotkey", mTimerStartHotkey).toString());
    setTimerPauseHotkey(settings.value("TimerPauseHotkey", mTimerPauseHotkey).toString());
    setTimerStopHotkey(settings.value("TimerStopHotkey", mTimerStopHotkey).toString());
    setTimerStopStrategy(TimerStopStrategyUtil::fromString(
        settings.value("TimerStopStrategy", TimerStopStrategyUtil::toString(mTimerStopStrategy)).toString()));
    setTimerUpdateInterval(settings.value("TimerUpdateInterval", mTimerUpdateInterval).toInt());
    setAutoTimerUpdateInterval(settings.value("AutoTimerUpdateInterval", mAutoTimerUpdateInterval).toInt());
    settings.endGroup();

    settings.beginGroup("Social");
    setDiscordShowRichPresence(settings.value("DiscordShowRichPresence", mDiscordShowRichPresence).toBool());
    settings.endGroup();

    settings.beginGroup("SuspendProcess");
    setSuspendAndResumeHotkey(settings.value("SuspendAndResumeHotkey", mSuspendAndResumeHotkey).toString());
    setSuspendAndResumeDuration(settings.value("SuspendAndResumeDuration", mSuspendAndResumeDuration).toInt());
    settings.endGroup();

    settings.beginGroup("CloseGame");
    setCloseGameImmediatelyHotkey(settings.value("CloseGameImmediatelyHotkey", mCloseGameImmediatelyHotkey).toString());
    settings.endGroup();

    settings.beginGroup("HotkeyMap");
    setHotkeyMaps(settings.value("HotkeyMaps", mHotkeyMaps).value<QList<QString>>());
    settings.endGroup();

    settings.beginGroup("Event");
    {
        settings.beginGroup("EventCmdMap");
        QMap<QString, QString> m;
        for (const auto& key : settings.allKeys()) {
            m[key] = settings.value(key).toString();
        }
        setEventCmdMap(m);
        settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Language");
    setLanguage(settings.value("Language", mLanguage).toString());
    settings.endGroup();

    settings.beginGroup("DeveloperOptions");
    setDebugMode(settings.value("DebugMode", mDebugMode).toBool());
    settings.endGroup();
}

void GlobalData::writeSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("StyleName", mStyleName);
    settings.setValue("AutoCheckUpdate", mAutoCheckUpdate);
    settings.setValue("Version", QApplication::applicationVersion());
    settings.setValue("IgnoredNewVersion", mIgnoredNewVersion);
    settings.setValue("MinimizeToTray", mMinimizeToTray);
    settings.setValue("TopMostWindowHotkey", mTopMostWindowHotkey);
    settings.setValue("ExcludeFromCapture", mExcludeFromCapture);
    settings.endGroup();

    settings.beginGroup("DisplayInfo");
    settings.setValue("DisplayInfoToolWindow", mDisplayInfoToolWindow);
    settings.setValue("DisplayInfoShow", mDisplayInfoShow);
    settings.setValue("DisplayInfoTouchable", mDisplayInfoTouchable);
    settings.setValue("DisplayInfoServer", mDisplayInfoServer);
    settings.setValue("DisplayInfoPosX", mDisplayInfoPos.x());
    settings.setValue("DisplayInfoPosY", mDisplayInfoPos.y());
    settings.setValue("DisplayInfoWidth", mDisplayInfoSize.width());
    settings.setValue("DisplayInfoHeight", mDisplayInfoSize.height());
    settings.setValue("DisplayInfoBackground", mDisplayInfoBackground.rgba());
    settings.endGroup();
    writeSubFuncSettingsMap(settings);

    settings.beginGroup("Crosshair");
    settings.setValue("CrosshairShow", mCrosshairShow);
    settings.setValue("CrosshairShowHotkey", mCrosshairShowHotkey);
    settings.setValue("CrosshairImage", mCrosshairImage);
    settings.setValue("CrosshairOffset", mCrosshairOffset);
    settings.setValue("CrosshairSize", mCrosshairSize);
    settings.setValue("CrosshairColor", mCrosshairColor);
    settings.setValue("CrosshairOpacity", mCrosshairOpacity);
    settings.setValue("CrosshairShadowColor", mCrosshairShadowColor);
    settings.setValue("CrosshairShadowBlurRadius", mCrosshairShadowBlurRadius);
    settings.setValue("CrosshairShadowOffset", mCrosshairShadowOffset);
    settings.setValue("CrosshairAttachWindowTitle", mCrosshairAttachWindowTitle);
    settings.endGroup();

    settings.beginGroup("Rtss");
    settings.setValue("RtssOverlay", mRtssOverlay);
    settings.setValue("RtssCrosshair", mRtssCrosshair);
    settings.setValue("RtssCrosshairText", mRtssCrosshairText);
    settings.setValue("RtssCrosshairX", mRtssCrosshairX);
    settings.setValue("RtssCrosshairY", mRtssCrosshairY);
    settings.setValue("RtssCrosshairSize", mRtssCrosshairSize);
    settings.setValue("RtssCrosshairColor", mRtssCrosshairColor);
    settings.endGroup();

    settings.beginGroup("Firewall");
    settings.setValue("FirewallStartHotkey", mFirewallStartHotkey);
    settings.setValue("FirewallStopHotkey", mFirewallStopHotkey);
    settings.setValue("FirewallStartSound", mFirewallStartSound);
    settings.setValue("FirewallStopSound", mFirewallStopSound);
    settings.setValue("FirewallErrorSound", mFirewallErrorSound);
    settings.setValue("FirewallPlaySound", mFirewallPlaySound);
    settings.setValue("FirewallAppPath", mFirewallAppPath);
    settings.setValue("FirewallDirection", mFirewallDirection);
    settings.setValue("FirewallProtocol", mFirewallProtocol);
    settings.endGroup();

    settings.beginGroup("NetworkAdapter");
    settings.setValue("SelectedNetworkAdapters", mSelectedNetworkAdapters);
    settings.setValue("NetworkAdaptersDisableHotkey", mNetworkAdaptersDisableHotkey);
    settings.setValue("NetworkAdaptersEnableHotkey", mNetworkAdaptersEnableHotkey);
    settings.setValue("NetworkAdaptersDisableSound", mNetworkAdaptersDisableSound);
    settings.setValue("NetworkAdaptersEnableSound", mNetworkAdaptersEnableSound);
    settings.endGroup();

    settings.beginGroup("MissionData");
    settings.setValue("MissionDataInterval", mMissionDataUpdateInterval);
    settings.setValue("MissionDataName", MissionDataNameUtil::toString(mMissionDataName));
    settings.setValue("RpRect", mRpRect);
    settings.setValue("Act3HeadshotStartHotkey", mAct3HeadshotStartHotkey);
    settings.setValue("Act3HeadshotStopHotkey", mAct3HeadshotStopHotkey);
    settings.setValue("Act3HeadshotStartSound", mAct3HeadshotStartSound);
    settings.setValue("Act3HeadshotStopSound", mAct3HeadshotStopSound);
    settings.endGroup();

    settings.beginGroup("Timer");
    settings.setValue("TimerStartHotkey", mTimerStartHotkey);
    settings.setValue("TimerPauseHotkey", mTimerPauseHotkey);
    settings.setValue("TimerStopHotkey", mTimerStopHotkey);
    settings.setValue("TimerStopStrategy", TimerStopStrategyUtil::toString(mTimerStopStrategy));
    settings.setValue("TimerUpdateInterval", mTimerUpdateInterval);
    settings.setValue("AutoTimerUpdateInterval", mAutoTimerUpdateInterval);
    settings.endGroup();

    settings.beginGroup("Social");
    settings.setValue("DiscordShowRichPresence", mDiscordShowRichPresence);
    settings.endGroup();

    settings.beginGroup("SuspendProcess");
    settings.setValue("SuspendAndResumeHotkey", mSuspendAndResumeHotkey);
    settings.setValue("SuspendAndResumeDuration", mSuspendAndResumeDuration);
    settings.endGroup();

    settings.beginGroup("CloseGame");
    settings.setValue("CloseGameImmediatelyHotkey", mCloseGameImmediatelyHotkey);
    settings.endGroup();

    settings.beginGroup("HotkeyMap");
    settings.setValue("HotkeyMaps", mHotkeyMaps);
    settings.endGroup();

    settings.beginGroup("Event");
    {
        settings.beginGroup("EventCmdMap");
        for (auto i = mEventCmdMap.constBegin(); i != mEventCmdMap.constEnd(); i++) {
            settings.setValue(i.key(), i.value());
        }
        settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Language");
    settings.setValue("Language", mLanguage);
    settings.endGroup();

    settings.beginGroup("DeveloperOptions");
    settings.setValue("DebugMode", mDebugMode);
    settings.endGroup();
}

void GlobalData::readSubFuncSettingsMap(QSettings& settings)
{
    QDisplayInfoSubFuncsMap displayInfoSubFunctions;
    for (auto i : mFuncs) {
        displayInfoSubFunctions[i] = new DisplayInfoSubFunctionItem(this);
    }
    if (settings.childGroups().contains("DisplayInfoSubFunctions", Qt::CaseInsensitive)) {
        settings.beginGroup("DisplayInfoSubFunctions");
    } else {
        settings.beginGroup("SubFunctions");
    }
    for (auto i = displayInfoSubFunctions.constBegin(); i != displayInfoSubFunctions.constEnd(); i++) {
        auto defaultValue = i.value();
        settings.beginGroup(DisplayInfoSubFunctionUtil::toString(i.key()));
        defaultValue->setDisplay(settings.value("Display", defaultValue->display()).toBool());
        defaultValue->setTextAlignment(Qt::Alignment(
            settings.value("TextAlignment", defaultValue->textAlignment().toInt()).toInt()));
        defaultValue->setTextSize(settings.value("TextSize", defaultValue->textSize()).toInt());
        defaultValue->setTextColor(settings.value("TextColor", defaultValue->textColor()).value<QColor>());
        defaultValue->setTextShadowColor(settings.value("TextShadowColor", defaultValue->textShadowColor())
                                             .value<QColor>());
        defaultValue->setTextShadowBlurRadius(settings.value("TextShadowBlurRadius", defaultValue->textShadowBlurRadius()).toInt());
        defaultValue->setTextShadowOffset(settings.value("TextShadowOffset", defaultValue->textShadowOffset()).value<QPointF>());
        defaultValue->setFontFamily(settings.value("FontFamily", defaultValue->fontFamily()).toString());
        defaultValue->setRtssDisplay(settings.value("RtssDisplay", defaultValue->rtssDisplay()).toBool());
        defaultValue->setRtssOsdText(settings.value("RtssOsdText", DisplayInfoSubFunctionUtil::defaultRtssOsdText(i.key())).toString());
        GlobalData::mDisplayInfoSubFunctions[i.key()] = defaultValue;
        settings.endGroup();
    }
    settings.endGroup();
}

void GlobalData::writeSubFuncSettingsMap(QSettings& settings)
{
    settings.beginGroup("DisplayInfoSubFunctions");
    for (auto i = mDisplayInfoSubFunctions.constBegin(); i != mDisplayInfoSubFunctions.constEnd(); i++) {
        auto currentValue = i.value();
        settings.beginGroup(DisplayInfoSubFunctionUtil::toString(i.key()));
        settings.setValue("Display", currentValue->display());
        settings.setValue("TextAlignment", currentValue->textAlignment().toInt());
        settings.setValue("TextSize", currentValue->textSize());
        settings.setValue("TextColor", currentValue->textColor());
        settings.setValue("TextShadowColor", currentValue->textShadowColor());
        settings.setValue("TextShadowBlurRadius", currentValue->textShadowBlurRadius());
        settings.setValue("TextShadowOffset", currentValue->textShadowOffset());
        settings.setValue("FontFamily", currentValue->fontFamily());
        settings.setValue("RtssDisplay", currentValue->rtssDisplay());
        settings.setValue("RtssOsdText", currentValue->rtssOsdText());
        settings.endGroup();
    }
    settings.endGroup();
    if (settings.childGroups().contains("SubFunctions", Qt::CaseInsensitive)) {
        settings.beginGroup("SubFunctions");
        settings.remove("");
        settings.endGroup();
    }
}

// ================================================================
bool GlobalData::minimizeToTray() const
{
    return mMinimizeToTray;
}

void GlobalData::setMinimizeToTray(bool newMinimizeToTray)
{
    if (mMinimizeToTray == newMinimizeToTray)
        return;
    mMinimizeToTray = newMinimizeToTray;
    emit minimizeToTrayChanged();
}

const QString& GlobalData::styleName() const
{
    return mStyleName;
}

void GlobalData::setStyleName(const QString& newStyleName)
{
    if (mStyleName == newStyleName)
        return;
    mStyleName = newStyleName;
    qApp->setStyle(mStyleName);
    emit styleNameChanged();
}

bool GlobalData::autoCheckUpdate() const
{
    return mAutoCheckUpdate;
}

void GlobalData::setAutoCheckUpdate(bool newAutoCheckUpdate)
{
    if (mAutoCheckUpdate == newAutoCheckUpdate)
        return;
    mAutoCheckUpdate = newAutoCheckUpdate;
    emit autoCheckUpdateChanged();
}

const QString& GlobalData::version() const
{
    return mVersion;
}

void GlobalData::setVersion(const QString& newVersion)
{
    if (mVersion == newVersion)
        return;
    mVersion = newVersion;
    emit versionChanged();
}

const QString& GlobalData::ignoredNewVersion() const
{
    return mIgnoredNewVersion;
}

void GlobalData::setIgnoredNewVersion(const QString& newIgnoredNewVersion)
{
    if (mIgnoredNewVersion == newIgnoredNewVersion)
        return;
    mIgnoredNewVersion = newIgnoredNewVersion;
    emit ignoredNewVersionChanged();
}

const QList<DisplayInfoSubFunction>& GlobalData::funcs() const
{
    return mFuncs;
}

const QDisplayInfoSubFuncsMap& GlobalData::displayInfoSubFunctions() const
{
    return mDisplayInfoSubFunctions;
}

bool GlobalData::displayInfoShow() const
{
    return mDisplayInfoShow;
}

void GlobalData::setDisplayInfoShow(bool newDisplayInfoShow)
{
    if (mDisplayInfoShow == newDisplayInfoShow)
        return;
    mDisplayInfoShow = newDisplayInfoShow;
    emit displayInfoShowChanged();
}

bool GlobalData::displayInfoTouchable() const
{
    return mDisplayInfoTouchable;
}

void GlobalData::setDisplayInfoTouchable(bool newDisplayInfoTouchable)
{
    if (mDisplayInfoTouchable == newDisplayInfoTouchable)
        return;
    mDisplayInfoTouchable = newDisplayInfoTouchable;
    emit displayInfoTouchableChanged();
}

bool GlobalData::displayInfoServer() const
{
    return mDisplayInfoServer;
}

void GlobalData::setDisplayInfoServer(bool newDisplayInfoServer)
{
    if (mDisplayInfoServer == newDisplayInfoServer)
        return;
    mDisplayInfoServer = newDisplayInfoServer;
    emit displayInfoServerChanged();
}

QPoint GlobalData::displayInfoPos() const
{
    return mDisplayInfoPos;
}

void GlobalData::setDisplayInfoPos(QPoint newDisplayInfoPos)
{
    if (mDisplayInfoPos == newDisplayInfoPos)
        return;
    mDisplayInfoPos = newDisplayInfoPos;
    emit displayInfoPosChanged();
}

const QSize& GlobalData::displayInfoSize() const
{
    return mDisplayInfoSize;
}

void GlobalData::setDisplayInfoSize(const QSize& newDisplayInfoSize)
{
    if (mDisplayInfoSize == newDisplayInfoSize)
        return;
    mDisplayInfoSize = newDisplayInfoSize;
    emit displayInfoSizeChanged();
}

const QColor& GlobalData::displayInfoBackground() const
{
    return mDisplayInfoBackground;
}

void GlobalData::setDisplayInfoBackground(const QColor& newDisplayInfoBackground)
{
    if (mDisplayInfoBackground == newDisplayInfoBackground)
        return;
    mDisplayInfoBackground = newDisplayInfoBackground;
    emit displayInfoBackgroundChanged();
}

const QString& GlobalData::firewallStartHotkey() const
{
    return mFirewallStartHotkey;
}

void GlobalData::setFirewallStartHotkey(const QString& newFirewallStartHotkey)
{
    if (mFirewallStartHotkey == newFirewallStartHotkey)
        return;
    mFirewallStartHotkey = newFirewallStartHotkey;
    emit firewallStartHotkeyChanged();
}

const QString& GlobalData::firewallStopHotkey() const
{
    return mFirewallStopHotkey;
}

void GlobalData::setFirewallStopHotkey(const QString& newFirewallStopHotkey)
{
    if (mFirewallStopHotkey == newFirewallStopHotkey)
        return;
    mFirewallStopHotkey = newFirewallStopHotkey;
    emit firewallStopHotkeyChanged();
}

const QString& GlobalData::firewallStartSound() const
{
    return mFirewallStartSound;
}

void GlobalData::setFirewallStartSound(const QString& newFirewallStartSound)
{
    if (mFirewallStartSound == newFirewallStartSound)
        return;
    mFirewallStartSound = newFirewallStartSound;
    emit firewallStartSoundChanged();
}

const QString& GlobalData::firewallStopSound() const
{
    return mFirewallStopSound;
}

void GlobalData::setFirewallStopSound(const QString& newFirewallStopSound)
{
    if (mFirewallStopSound == newFirewallStopSound)
        return;
    mFirewallStopSound = newFirewallStopSound;
    emit firewallStopSoundChanged();
}

const QString& GlobalData::firewallErrorSound() const
{
    return mFirewallErrorSound;
}

void GlobalData::setFirewallErrorSound(const QString& newFirewallErrorSound)
{
    if (mFirewallErrorSound == newFirewallErrorSound)
        return;
    mFirewallErrorSound = newFirewallErrorSound;
    emit firewallErrorSoundChanged();
}

bool GlobalData::firewallPlaySound() const
{
    return mFirewallPlaySound;
}

void GlobalData::setFirewallPlaySound(bool newFirewallPlaySound)
{
    if (mFirewallPlaySound == newFirewallPlaySound)
        return;
    mFirewallPlaySound = newFirewallPlaySound;
    emit firewallPlaySoundChanged();
}

const QString& GlobalData::firewallAppPath() const
{
    return mFirewallAppPath;
}

void GlobalData::setFirewallAppPath(const QString& newFirewallAppPath)
{
    if (mFirewallAppPath == newFirewallAppPath)
        return;
    mFirewallAppPath = newFirewallAppPath;
    emit firewallAppPathChanged();
}

int GlobalData::firewallDirection() const
{
    return mFirewallDirection;
}

void GlobalData::setFirewallDirection(int newFirewallDirection)
{
    if (mFirewallDirection == newFirewallDirection)
        return;
    mFirewallDirection = newFirewallDirection;
    emit firewallDirectionChanged();
}

int GlobalData::missionDataUpdateInterval() const
{
    return mMissionDataUpdateInterval;
}

void GlobalData::setMissionDataUpdateInterval(int newMissionDataUpdateInterval)
{
    if (mMissionDataUpdateInterval == newMissionDataUpdateInterval)
        return;
    mMissionDataUpdateInterval = newMissionDataUpdateInterval;
    emit missionDataUpdateIntervalChanged();
}

const QString& GlobalData::timerStartHotkey() const
{
    return mTimerStartHotkey;
}

void GlobalData::setTimerStartHotkey(const QString& newTimerStartHotkey)
{
    if (mTimerStartHotkey == newTimerStartHotkey)
        return;
    mTimerStartHotkey = newTimerStartHotkey;
    emit timerStartHotkeyChanged();
}

const QString& GlobalData::timerPauseHotkey() const
{
    return mTimerPauseHotkey;
}

void GlobalData::setTimerPauseHotkey(const QString& newTimerPauseHotkey)
{
    if (mTimerPauseHotkey == newTimerPauseHotkey)
        return;
    mTimerPauseHotkey = newTimerPauseHotkey;
    emit timerPauseHotkeyChanged();
}

const QString& GlobalData::timerStopHotkey() const
{
    return mTimerStopHotkey;
}

void GlobalData::setTimerStopHotkey(const QString& newTimerStopHotkey)
{
    if (mTimerStopHotkey == newTimerStopHotkey)
        return;
    mTimerStopHotkey = newTimerStopHotkey;
    emit timerStopHotkeyChanged();
}

const QList<TimerStopStrategy>& GlobalData::timerStopStrategies() const
{
    return mTimerStopStrategies;
}

TimerStopStrategy GlobalData::timerStopStrategy() const
{
    return mTimerStopStrategy;
}

void GlobalData::setTimerStopStrategy(TimerStopStrategy newTimerStopStrategy)
{
    if (mTimerStopStrategy == newTimerStopStrategy)
        return;
    mTimerStopStrategy = newTimerStopStrategy;
    emit timerStopStrategyChanged();
}

int GlobalData::timerUpdateInterval() const
{
    return mTimerUpdateInterval;
}

void GlobalData::setTimerUpdateInterval(int newTimerUpdateInterval)
{
    if (mTimerUpdateInterval == newTimerUpdateInterval)
        return;
    mTimerUpdateInterval = newTimerUpdateInterval;
    emit timerUpdateIntervalChanged();
}

const QString& GlobalData::language() const
{
    return mLanguage;
}

void GlobalData::setLanguage(const QString& newLanguage)
{
    if (mLanguage == newLanguage)
        return;
    mLanguage = newLanguage;
    emit languageChanged();
}

int GlobalData::serverHttpPort() const
{
    return mServerHttpPort;
}

void GlobalData::setServerHttpPort(int newServerHttpPort)
{
    if (mServerHttpPort == newServerHttpPort)
        return;
    mServerHttpPort = newServerHttpPort;
    emit serverHttpPortChanged();
}

int GlobalData::serverWebsocketPort() const
{
    return mServerWebsocketPort;
}

void GlobalData::setServerWebsocketPort(int newServerWebsocketPort)
{
    if (mServerWebsocketPort == newServerWebsocketPort)
        return;
    mServerWebsocketPort = newServerWebsocketPort;
    emit serverWebsocketPortChanged();
}

bool GlobalData::discordShowRichPresence() const
{
    return mDiscordShowRichPresence;
}

void GlobalData::setDiscordShowRichPresence(bool newDiscordShowRichPresence)
{
    if (mDiscordShowRichPresence == newDiscordShowRichPresence)
        return;
    mDiscordShowRichPresence = newDiscordShowRichPresence;
    emit discordShowRichPresenceChanged();
}

const QString& GlobalData::closeGameImmediatelyHotkey() const
{
    return mCloseGameImmediatelyHotkey;
}

void GlobalData::setCloseGameImmediatelyHotkey(const QString& newCloseGameImmediatelyHotkey)
{
    if (mCloseGameImmediatelyHotkey == newCloseGameImmediatelyHotkey)
        return;
    mCloseGameImmediatelyHotkey = newCloseGameImmediatelyHotkey;
    emit closeGameImmediatelyHotkeyChanged();
}

int GlobalData::autoTimerUpdateInterval() const
{
    return mAutoTimerUpdateInterval;
}

void GlobalData::setAutoTimerUpdateInterval(int newAutoTimerUpdateInterval)
{
    if (mAutoTimerUpdateInterval == newAutoTimerUpdateInterval)
        return;
    mAutoTimerUpdateInterval = newAutoTimerUpdateInterval;
    emit autoTimerUpdateIntervalChanged();
}

QList<MissionDataNameUtil::MissionDataName> GlobalData::missionDataNames() const
{
    return mMissionDataNames;
}

MissionDataNameUtil::MissionDataName GlobalData::missionDataName() const
{
    return mMissionDataName;
}

void GlobalData::setMissionDataName(MissionDataNameUtil::MissionDataName newMissionDataName)
{
    if (mMissionDataName == newMissionDataName)
        return;
    mMissionDataName = newMissionDataName;
    emit missionDataNameChanged();
}

bool GlobalData::debugMode() const
{
    return mDebugMode;
}

void GlobalData::setDebugMode(bool newDebugMode)
{
    if (mDebugMode == newDebugMode)
        return;
    mDebugMode = newDebugMode;
    emit debugModeChanged();
}

bool GlobalData::displayInfoToolWindow() const
{
    return mDisplayInfoToolWindow;
}

void GlobalData::setDisplayInfoToolWindow(bool newDisplayInfoToolWindow)
{
    if (mDisplayInfoToolWindow == newDisplayInfoToolWindow)
        return;
    mDisplayInfoToolWindow = newDisplayInfoToolWindow;
    emit displayInfoToolWindowChanged();
}

QString GlobalData::suspendAndResumeHotkey() const
{
    return mSuspendAndResumeHotkey;
}

void GlobalData::setSuspendAndResumeHotkey(const QString& newSuspendAndResumeHotkey)
{
    if (mSuspendAndResumeHotkey == newSuspendAndResumeHotkey)
        return;
    mSuspendAndResumeHotkey = newSuspendAndResumeHotkey;
    emit suspendAndResumeHotkeyChanged();
}

int GlobalData::suspendAndResumeDuration() const
{
    return mSuspendAndResumeDuration;
}

void GlobalData::setSuspendAndResumeDuration(int newSuspendAndResumeDuration)
{
    if (mSuspendAndResumeDuration == newSuspendAndResumeDuration)
        return;
    mSuspendAndResumeDuration = newSuspendAndResumeDuration;
    emit suspendAndResumeDurationChanged();
}

QRect GlobalData::rpRect() const
{
    return mRpRect;
}

void GlobalData::setRpRect(const QRect& newRpRect)
{
    if (mRpRect == newRpRect)
        return;
    mRpRect = newRpRect;
    emit rpRectChanged();
}

QString GlobalData::act3HeadshotStartHotkey() const
{
    return mAct3HeadshotStartHotkey;
}

void GlobalData::setAct3HeadshotStartHotkey(const QString& newAct3HeadshotStartHotkey)
{
    if (mAct3HeadshotStartHotkey == newAct3HeadshotStartHotkey)
        return;
    mAct3HeadshotStartHotkey = newAct3HeadshotStartHotkey;
    emit act3HeadshotStartHotkeyChanged();
}

QString GlobalData::act3HeadshotStopHotkey() const
{
    return mAct3HeadshotStopHotkey;
}

void GlobalData::setAct3HeadshotStopHotkey(const QString& newAct3HeadshotStopHotkey)
{
    if (mAct3HeadshotStopHotkey == newAct3HeadshotStopHotkey)
        return;
    mAct3HeadshotStopHotkey = newAct3HeadshotStopHotkey;
    emit act3HeadshotStopHotkeyChanged();
}

QString GlobalData::act3HeadshotStartSound() const
{
    return mAct3HeadshotStartSound;
}

void GlobalData::setAct3HeadshotStartSound(const QString& newAct3HeadshotStartSound)
{
    if (mAct3HeadshotStartSound == newAct3HeadshotStartSound)
        return;
    mAct3HeadshotStartSound = newAct3HeadshotStartSound;
    emit act3HeadshotStartSoundChanged();
}

QString GlobalData::act3HeadshotStopSound() const
{
    return mAct3HeadshotStopSound;
}

void GlobalData::setAct3HeadshotStopSound(const QString& newAct3HeadshotStopSound)
{
    if (mAct3HeadshotStopSound == newAct3HeadshotStopSound)
        return;
    mAct3HeadshotStopSound = newAct3HeadshotStopSound;
    emit act3HeadshotStopSoundChanged();
}

bool GlobalData::rtssOverlay() const
{
    return mRtssOverlay;
}

void GlobalData::setRtssOverlay(bool newRtssOverlay)
{
    if (mRtssOverlay == newRtssOverlay)
        return;
    mRtssOverlay = newRtssOverlay;
    emit rtssOverlayChanged();
}

QList<QString> GlobalData::selectedNetworkAdapters() const
{
    return mSelectedNetworkAdapters;
}

void GlobalData::setSelectedNetworkAdapters(const QList<QString>& newSelectedNetworkAdapters)
{
    if (mSelectedNetworkAdapters == newSelectedNetworkAdapters)
        return;
    mSelectedNetworkAdapters = newSelectedNetworkAdapters;
    emit selectedNetworkAdaptersChanged();
}

QString GlobalData::networkAdaptersEnableHotkey() const
{
    return mNetworkAdaptersEnableHotkey;
}

void GlobalData::setNetworkAdaptersEnableHotkey(const QString& newNetworkAdaptersEnableHotkey)
{
    if (mNetworkAdaptersEnableHotkey == newNetworkAdaptersEnableHotkey)
        return;
    mNetworkAdaptersEnableHotkey = newNetworkAdaptersEnableHotkey;
    emit networkAdaptersEnableHotkeyChanged();
}

QString GlobalData::networkAdaptersDisableHotkey() const
{
    return mNetworkAdaptersDisableHotkey;
}

void GlobalData::setNetworkAdaptersDisableHotkey(const QString& newNetworkAdaptersDisableHotkey)
{
    if (mNetworkAdaptersDisableHotkey == newNetworkAdaptersDisableHotkey)
        return;
    mNetworkAdaptersDisableHotkey = newNetworkAdaptersDisableHotkey;
    emit networkAdaptersDisableHotkeyChanged();
}

QString GlobalData::networkAdaptersEnableSound() const
{
    return mNetworkAdaptersEnableSound;
}

void GlobalData::setNetworkAdaptersEnableSound(const QString& newNetworkAdaptersEnableSound)
{
    if (mNetworkAdaptersEnableSound == newNetworkAdaptersEnableSound)
        return;
    mNetworkAdaptersEnableSound = newNetworkAdaptersEnableSound;
    emit networkAdaptersEnableSoundChanged();
}

QString GlobalData::networkAdaptersDisableSound() const
{
    return mNetworkAdaptersDisableSound;
}

void GlobalData::setNetworkAdaptersDisableSound(const QString& newNetworkAdaptersDisableSound)
{
    if (mNetworkAdaptersDisableSound == newNetworkAdaptersDisableSound)
        return;
    mNetworkAdaptersDisableSound = newNetworkAdaptersDisableSound;
    emit networkAdaptersDisableSoundChanged();
}

bool GlobalData::rtssCrosshair() const
{
    return mRtssCrosshair;
}

void GlobalData::setRtssCrosshair(bool newRtssCrosshair)
{
    if (mRtssCrosshair == newRtssCrosshair)
        return;
    mRtssCrosshair = newRtssCrosshair;
    emit rtssCrosshairChanged();
}

QString GlobalData::rtssCrosshairText() const
{
    return mRtssCrosshairText;
}

void GlobalData::setRtssCrosshairText(const QString& newRtssCrosshairText)
{
    if (mRtssCrosshairText == newRtssCrosshairText)
        return;
    mRtssCrosshairText = newRtssCrosshairText;
    emit rtssCrosshairTextChanged();
}

int GlobalData::rtssCrosshairX() const
{
    return mRtssCrosshairX;
}

void GlobalData::setRtssCrosshairX(int newRtssCrosshairX)
{
    if (mRtssCrosshairX == newRtssCrosshairX)
        return;
    mRtssCrosshairX = newRtssCrosshairX;
    emit rtssCrosshairXChanged();
}

int GlobalData::rtssCrosshairY() const
{
    return mRtssCrosshairY;
}

void GlobalData::setRtssCrosshairY(int newRtssCrosshairY)
{
    if (mRtssCrosshairY == newRtssCrosshairY)
        return;
    mRtssCrosshairY = newRtssCrosshairY;
    emit rtssCrosshairYChanged();
}

int GlobalData::rtssCrosshairSize() const
{
    return mRtssCrosshairSize;
}

void GlobalData::setRtssCrosshairSize(int newRtssCrosshairSize)
{
    if (mRtssCrosshairSize == newRtssCrosshairSize)
        return;
    mRtssCrosshairSize = newRtssCrosshairSize;
    emit rtssCrosshairSizeChanged();
}

QColor GlobalData::rtssCrosshairColor() const
{
    return mRtssCrosshairColor;
}

void GlobalData::setRtssCrosshairColor(QColor newRtssCrosshairColor)
{
    if (mRtssCrosshairColor == newRtssCrosshairColor)
        return;
    mRtssCrosshairColor = newRtssCrosshairColor;
    emit rtssCrosshairColorChanged();
}

QString GlobalData::topMostWindowHotkey() const
{
    return mTopMostWindowHotkey;
}

void GlobalData::setTopMostWindowHotkey(const QString& newShowToolHotkey)
{
    if (mTopMostWindowHotkey == newShowToolHotkey)
        return;
    mTopMostWindowHotkey = newShowToolHotkey;
    emit topMostWindowHotkeyChanged();
}

int GlobalData::firewallProtocol() const
{
    return mFirewallProtocol;
}

void GlobalData::setFirewallProtocol(int newFirewallProtocol)
{
    if (mFirewallProtocol == newFirewallProtocol || newFirewallProtocol > 255 || newFirewallProtocol < -1)
        return;
    mFirewallProtocol = newFirewallProtocol;
    emit firewallProtocolChanged();
}

QList<QString> GlobalData::hotkeyMaps() const
{
    return mHotkeyMaps;
}

void GlobalData::setHotkeyMaps(const QList<QString>& newHotkeyMaps)
{
    if (mHotkeyMaps == newHotkeyMaps)
        return;
    mHotkeyMaps = newHotkeyMaps;
    emit hotkeyMapsChanged();
}

QMap<QString, QString>& GlobalData::eventCmdMap()
{
    return mEventCmdMap;
}

void GlobalData::setEventCmdMap(const QMap<QString, QString>& newEventCmdMap)
{
    if (mEventCmdMap == newEventCmdMap)
        return;
    mEventCmdMap = newEventCmdMap;
    emit eventCmdMapChanged();
}

QPoint GlobalData::crosshairOffset() const
{
    return mCrosshairOffset;
}

void GlobalData::setCrosshairOffset(QPoint newCrosshairOffset)
{
    if (mCrosshairOffset == newCrosshairOffset)
        return;
    mCrosshairOffset = newCrosshairOffset;
    emit crosshairOffsetChanged();
}

QSize GlobalData::crosshairSize() const
{
    return mCrosshairSize;
}

void GlobalData::setCrosshairSize(const QSize& newCrosshairSize)
{
    if (mCrosshairSize == newCrosshairSize)
        return;
    mCrosshairSize = newCrosshairSize;
    emit crosshairSizeChanged();
}

QString GlobalData::crosshairImage() const
{
    return mCrosshairImage;
}

void GlobalData::setCrosshairImage(const QString& newCrosshairImage)
{
    if (mCrosshairImage == newCrosshairImage)
        return;
    mCrosshairImage = newCrosshairImage;
    emit crosshairImageChanged();
}

bool GlobalData::crosshairShow() const
{
    return mCrosshairShow;
}

void GlobalData::setCrosshairShow(bool newCrosshairShow)
{
    if (mCrosshairShow == newCrosshairShow)
        return;
    mCrosshairShow = newCrosshairShow;
    emit crosshairShowChanged();
}

QColor GlobalData::crosshairShadowColor() const
{
    return mCrosshairShadowColor;
}

void GlobalData::setCrosshairShadowColor(const QColor& newCrosshairShadowColor)
{
    if (mCrosshairShadowColor == newCrosshairShadowColor)
        return;
    mCrosshairShadowColor = newCrosshairShadowColor;
    emit crosshairShadowColorChanged();
}

int GlobalData::crosshairShadowBlurRadius() const
{
    return mCrosshairShadowBlurRadius;
}

void GlobalData::setCrosshairShadowBlurRadius(int newCrosshairShadowBlurRadius)
{
    if (mCrosshairShadowBlurRadius == newCrosshairShadowBlurRadius)
        return;
    mCrosshairShadowBlurRadius = newCrosshairShadowBlurRadius;
    emit crosshairShadowBlurRadiusChanged();
}

QPointF GlobalData::crosshairShadowOffset() const
{
    return mCrosshairShadowOffset;
}

void GlobalData::setCrosshairShadowOffset(QPointF newCrosshairShadowOffset)
{
    if (mCrosshairShadowOffset == newCrosshairShadowOffset)
        return;
    mCrosshairShadowOffset = newCrosshairShadowOffset;
    emit crosshairShadowOffsetChanged();
}

bool GlobalData::excludeFromCapture() const
{
    return mExcludeFromCapture;
}

void GlobalData::setExcludeFromCapture(bool newExcludeFromCapture)
{
    if (mExcludeFromCapture == newExcludeFromCapture)
        return;
    mExcludeFromCapture = newExcludeFromCapture;
    emit excludeFromCaptureChanged();
}

QString GlobalData::crosshairShowHotkey() const
{
    return mCrosshairShowHotkey;
}

void GlobalData::setCrosshairShowHotkey(const QString& newCrosshairShowHotkey)
{
    if (mCrosshairShowHotkey == newCrosshairShowHotkey)
        return;
    mCrosshairShowHotkey = newCrosshairShowHotkey;
    emit crosshairShowHotkeyChanged();
}

QColor GlobalData::crosshairColor() const
{
    return mCrosshairColor;
}

void GlobalData::setCrosshairColor(const QColor& newCrosshairColor)
{
    if (mCrosshairColor == newCrosshairColor)
        return;
    mCrosshairColor = newCrosshairColor;
    emit crosshairColorChanged();
}

double GlobalData::crosshairOpacity() const
{
    return mCrosshairOpacity;
}

void GlobalData::setCrosshairOpacity(double newCrosshairOpacity)
{
    if (qFuzzyCompare(mCrosshairOpacity, newCrosshairOpacity))
        return;
    mCrosshairOpacity = newCrosshairOpacity;
    emit crosshairOpacityChanged();
}

QString GlobalData::crosshairAttachWindowTitle() const
{
    return mCrosshairAttachWindowTitle;
}

void GlobalData::setCrosshairAttachWindowTitle(const QString& newCrosshairAttachWindowTitle)
{
    if (mCrosshairAttachWindowTitle == newCrosshairAttachWindowTitle)
        return;
    mCrosshairAttachWindowTitle = newCrosshairAttachWindowTitle;
    emit crosshairAttachWindowTitleChanged();
}
