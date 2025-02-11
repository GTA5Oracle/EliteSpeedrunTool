#include "HotkeyUtil.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include <QDebug>
#include <hidusage.h>

Q_GLOBAL_STATIC(HotkeyUtil, hotkeyUtilInstance)

QHotkey::QHotkey(QKeySequence keySeq, bool autoRegister, QObject* parent)
    : keySeq { keySeq }
    , QObject { parent }
{
    if (autoRegister) {
        hotkeyUtil->registerHotkey(this);
    }
    auto hotkey = keySeq[0];
    nativeKeycode = HotkeyUtil::escapeNumPad(HotkeyUtil::nativeKeycode(hotkey.key(), nativeKeycodeOk));
    nativeModifiers = HotkeyUtil::nativeModifiers(hotkey.keyboardModifiers());
}

QHotkey::~QHotkey()
{
    hotkeyUtil->unregisterHotkey(this);
}

void QHotkey::setRegistered(bool registered)
{
    if (registered) {
        hotkeyUtil->registerHotkey(this);
    } else {
        hotkeyUtil->unregisterHotkey(this);
    }
}

void QHotkey::emitSignal()
{
    emit activated();
}

// -------------------------------
QHotkeyMap::QHotkeyMap(QKeySequence keySeq, QObject* parent)
    : QHotkey { keySeq, false, parent }
{
    auto qModifiers = keySeq[0].keyboardModifiers();
    if (qModifiers & Qt::ControlModifier)
        nativeModifiers << VK_CONTROL;
    if (qModifiers & Qt::ShiftModifier)
        nativeModifiers << VK_SHIFT;
    if (qModifiers & Qt::AltModifier)
        nativeModifiers << VK_MENU;

    ZeroMemory(&downInput, sizeof(INPUT));
    downInput.type = INPUT_KEYBOARD;
    downInput.ki.wVk = nativeKeycode;
    downInput.ki.wScan = MapVirtualKey(nativeKeycode, MAPVK_VK_TO_VSC);

    ZeroMemory(&upInput, sizeof(INPUT));
    upInput.type = INPUT_KEYBOARD;
    upInput.ki.wVk = nativeKeycode;
    upInput.ki.wScan = MapVirtualKey(nativeKeycode, MAPVK_VK_TO_VSC);
    upInput.ki.dwFlags = KEYEVENTF_KEYUP;
}

void QHotkeyMap::sendInput()
{
    // if (globalData->debugMode()) {
    //     qInfo() << "QHotkeyMap::sendInput" << keySeq.toString();
    // }
    // // 模拟按下第一个键
    // keybd_event(0, 0, KEYEVENTF_SCANCODE, 0); // 先按下不松开
    // keybd_event(VK_F5, 0, 0, 0); // 按下第一个键
    // // 模拟松开所有按键
    // keybd_event(0, 0, KEYEVENTF_SCANCODE, KEYEVENTF_KEYUP);

    SendInput(1, &downInput, sizeof(INPUT));
    SendInput(1, &upInput, sizeof(INPUT));
}

// -------------------------------
HotkeyUtil::HotkeyUtil(QWidget* parent)
    : QWidget { parent }
{
    qInfo() << "HotkeyUtil initializing...";
    registerRawInputDevice();

    connect(globalData, &GlobalData::hotkeyMapsChanged, this, [this]() {
        initHotkeyMaps();
    });
    initHotkeyMaps();

    hotkeyRedirector->show();
    hotkeyRedirector->hide();
}

HotkeyUtil* HotkeyUtil::instance()
{
    return hotkeyUtilInstance;
}

void HotkeyUtil::registerHotkey(QHotkey* key)
{
    if (!hotkeys.contains(key)) {
        hotkeys << key;
    }
}

void HotkeyUtil::unregisterHotkey(QHotkey* key)
{
    hotkeys.removeOne(key);
}

void HotkeyUtil::keyDown(DWORD key)
{
    if (isModifier(key)) {
        if (globalData->debugMode()) {
            qDebug() << "isModifier" << key;
        }
        modifiers |= modifierVkToMod(key);
        return;
    }

    key = escapeNumPad(key);
    HWND hwnd = reinterpret_cast<HWND>(hotkeyRedirector->winId());
    HWND gtaHwnd = memoryUtil->getGtaWindowHwnd();
    auto foregroundWindow = GetForegroundWindow();
    if (globalData->debugMode()) {
        qDebug() << "key" << key << "foregroundWindow" << foregroundWindow << "HWND" << hwnd << "GTAHWND" << gtaHwnd;
    }
    if (hotkeyMapEnabled && foregroundWindow == gtaHwnd) {
        for (const auto& hotkeySeqMap : hotkeyMaps) {
            QHotkey* hotkey = hotkeySeqMap;
            // if (globalData->debugMode()) {
            //     qDebug() << "nativeModifiers" << hotkey->nativeModifiers
            //              << "modifiers" << modifiers
            //              << "nativeKeycodeOk" << hotkeySeqMap->nativeKeycodeOk
            //              << "nativeKeycode" << hotkeySeqMap->nativeKeycode;
            // }
            if (hotkey->nativeModifiers != modifiers
                || !hotkeySeqMap->nativeKeycodeOk
                || hotkeySeqMap->nativeKeycode != key) {
                continue;
            }
            if (isRegisteredInSystem(hotkey->keySeq[0])) {
                SetParent(hwnd, gtaHwnd);
                hotkeyRedirector->show();
                SetForegroundWindow(hwnd);
                hotkeySeqMap->sendInput();
                SetForegroundWindow(gtaHwnd);
                hotkeyRedirector->hide();
                return;
            } else {
                qInfo() << "keyDown: This hotkey is not registered in the system!";
            }
        }
    }

    for (const auto& hotkeySeq : hotkeys) {
        if (globalData->debugMode()) {
            qInfo() << "hotkeySeq->nativeModifiers: " << hotkeySeq->nativeModifiers
                    << "modifiers: " << modifiers
                    << "hotkeySeq->nativeKeycodeOk: " << hotkeySeq->nativeKeycodeOk
                    << "hotkeySeq->nativeKeycode" << hotkeySeq->nativeKeycode
                    << "key: " << key;
        }
        if (hotkeySeq->nativeModifiers != modifiers
            || !hotkeySeq->nativeKeycodeOk
            || hotkeySeq->nativeKeycode != key) {
            continue;
        }
        if (!hotkeySeq->isPressed) {
            hotkeySeq->isPressed = true;
            hotkeySeq->emitSignal();
        } else {
            qInfo() << "This key already pressed, just skip";
            break;
        }
    }
}

void HotkeyUtil::keyUp(DWORD key)
{
    if (isModifier(key)) {
        modifiers &= ~modifierVkToMod(key);
        for (const auto& hotkeySeq : hotkeys) {
            if (hotkeySeq->nativeModifiers != modifiers) {
                continue;
            }
            hotkeySeq->isPressed = false;
        }
        return;
    }

    key = escapeNumPad(key);
    for (const auto& hotkeySeq : hotkeys) {
        if (!hotkeySeq->isPressed
            || !hotkeySeq->nativeKeycodeOk
            || hotkeySeq->nativeKeycode != key) {
            continue;
        }
        hotkeySeq->isPressed = false;
    }
}

bool HotkeyUtil::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_INPUT) {
        processRawInput(msg->lParam);
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void HotkeyUtil::closeEvent(QCloseEvent* event)
{
    if (hotkeyRedirector->close()) {
        hotkeyRedirector->deleteLater();
    }
    QWidget::closeEvent(event);
}

void HotkeyUtil::initHotkeyMaps()
{
    qDebug() << "initHotkeyMaps";
    for (auto hotkeySeqMap : hotkeyMaps) {
        delete hotkeySeqMap;
    }
    hotkeyMaps.clear();
    for (const auto& hotkeySeqMap : globalData->hotkeyMaps()) {
        hotkeyMaps << new QHotkeyMap(QKeySequence(hotkeySeqMap));
    }
}

void HotkeyUtil::setHotkeyMapEnabled(bool newHotkeyMapEnabled)
{
    hotkeyMapEnabled = newHotkeyMapEnabled;
}

bool HotkeyUtil::isRegisteredInSystem(QKeyCombination key)
{
    auto hwnd = reinterpret_cast<HWND>(winId());
    bool nativeKeycodeOk;
    if (RegisterHotKey(hwnd, 0, nativeModifiers(key.keyboardModifiers()), nativeKeycode(key.key(), nativeKeycodeOk))) {
        UnregisterHotKey(hwnd, 0);
        return false;
    } else {
        return true;
    }
}

bool HotkeyUtil::registerRawInputDevice()
{
    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawinputdevice
    RAWINPUTDEVICE rid;
    rid.usUsagePage = HID_USAGE_PAGE_GENERIC; // Generic Desktop Controls
    rid.usUsage = HID_USAGE_GENERIC_KEYBOARD; // Keyboard
    rid.dwFlags = RIDEV_INPUTSINK; // Enables the caller to receive the input even when the caller is not in the foreground
    rid.hwndTarget = reinterpret_cast<HWND>(winId()); // Window hwnd

    return RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

void HotkeyUtil::processRawInput(LPARAM lParam)
{
    RAWINPUT inputBuffer;
    UINT rawInputSize = sizeof(inputBuffer);
    GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, &inputBuffer, &rawInputSize, sizeof(RAWINPUTHEADER));

    // Parse RAWINPUT
    if (inputBuffer.header.dwType == RIM_TYPEKEYBOARD) {
        RAWKEYBOARD keyboard = inputBuffer.data.keyboard;
        if (keyboard.Flags & RI_KEY_BREAK) { // Key Down / Key Up
            keyUp(keyboard.VKey);
        } else {
            keyDown(keyboard.VKey);
        }
    }
}

short HotkeyUtil::nativeKeycode(Qt::Key keycode, bool& ok)
{
    ok = true;
    // find key from switch/case --> Only finds a very small subset of keys
    switch (keycode) {
    case Qt::Key_Escape:
        return VK_ESCAPE;
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        return VK_TAB;
    case Qt::Key_Backspace:
        return VK_BACK;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return VK_RETURN;
    case Qt::Key_Insert:
        return VK_INSERT;
    case Qt::Key_Delete:
        return VK_DELETE;
    case Qt::Key_Pause:
        return VK_PAUSE;
    case Qt::Key_Print:
        return VK_PRINT;
    case Qt::Key_Clear:
        return VK_CLEAR;
    case Qt::Key_Home:
        return VK_HOME;
    case Qt::Key_End:
        return VK_END;
    case Qt::Key_Left:
        return VK_LEFT;
    case Qt::Key_Up:
        return VK_UP;
    case Qt::Key_Right:
        return VK_RIGHT;
    case Qt::Key_Down:
        return VK_DOWN;
    case Qt::Key_PageUp:
        return VK_PRIOR;
    case Qt::Key_PageDown:
        return VK_NEXT;
    case Qt::Key_CapsLock:
        return VK_CAPITAL;
    case Qt::Key_NumLock:
        return VK_NUMLOCK;
    case Qt::Key_ScrollLock:
        return VK_SCROLL;

    case Qt::Key_F1:
        return VK_F1;
    case Qt::Key_F2:
        return VK_F2;
    case Qt::Key_F3:
        return VK_F3;
    case Qt::Key_F4:
        return VK_F4;
    case Qt::Key_F5:
        return VK_F5;
    case Qt::Key_F6:
        return VK_F6;
    case Qt::Key_F7:
        return VK_F7;
    case Qt::Key_F8:
        return VK_F8;
    case Qt::Key_F9:
        return VK_F9;
    case Qt::Key_F10:
        return VK_F10;
    case Qt::Key_F11:
        return VK_F11;
    case Qt::Key_F12:
        return VK_F12;
    case Qt::Key_F13:
        return VK_F13;
    case Qt::Key_F14:
        return VK_F14;
    case Qt::Key_F15:
        return VK_F15;
    case Qt::Key_F16:
        return VK_F16;
    case Qt::Key_F17:
        return VK_F17;
    case Qt::Key_F18:
        return VK_F18;
    case Qt::Key_F19:
        return VK_F19;
    case Qt::Key_F20:
        return VK_F20;
    case Qt::Key_F21:
        return VK_F21;
    case Qt::Key_F22:
        return VK_F22;
    case Qt::Key_F23:
        return VK_F23;
    case Qt::Key_F24:
        return VK_F24;

    case Qt::Key_Menu:
        return VK_APPS;
    case Qt::Key_Help:
        return VK_HELP;
    case Qt::Key_MediaNext:
        return VK_MEDIA_NEXT_TRACK;
    case Qt::Key_MediaPrevious:
        return VK_MEDIA_PREV_TRACK;
    case Qt::Key_MediaPlay:
        return VK_MEDIA_PLAY_PAUSE;
    case Qt::Key_MediaStop:
        return VK_MEDIA_STOP;
    case Qt::Key_VolumeDown:
        return VK_VOLUME_DOWN;
    case Qt::Key_VolumeUp:
        return VK_VOLUME_UP;
    case Qt::Key_VolumeMute:
        return VK_VOLUME_MUTE;
    case Qt::Key_Mode_switch:
        return VK_MODECHANGE;
    case Qt::Key_Select:
        return VK_SELECT;
    case Qt::Key_Printer:
        return VK_PRINT;
    case Qt::Key_Execute:
        return VK_EXECUTE;
    case Qt::Key_Sleep:
        return VK_SLEEP;
    case Qt::Key_Period:
        return VK_DECIMAL;
    case Qt::Key_Play:
        return VK_PLAY;
    case Qt::Key_Cancel:
        return VK_CANCEL;
    case Qt::Key_Plus:
        return VK_ADD;
    case Qt::Key_Minus:
        return VK_SUBTRACT;
    case Qt::Key_Slash:
        return VK_DIVIDE;
    case Qt::Key_Asterisk:
        return VK_MULTIPLY;

    case Qt::Key_Forward:
        return VK_BROWSER_FORWARD;
    case Qt::Key_Refresh:
        return VK_BROWSER_REFRESH;
    case Qt::Key_Stop:
        return VK_BROWSER_STOP;
    case Qt::Key_Search:
        return VK_BROWSER_SEARCH;
    case Qt::Key_Favorites:
        return VK_BROWSER_FAVORITES;
    case Qt::Key_HomePage:
        return VK_BROWSER_HOME;

    case Qt::Key_LaunchMail:
        return VK_LAUNCH_MAIL;
    case Qt::Key_LaunchMedia:
        return VK_LAUNCH_MEDIA_SELECT;
    case Qt::Key_Launch0:
        return VK_LAUNCH_APP1;
    case Qt::Key_Launch1:
        return VK_LAUNCH_APP2;

    case Qt::Key_Massyo:
        return VK_OEM_FJ_MASSHOU;
    case Qt::Key_Touroku:
        return VK_OEM_FJ_TOUROKU;

    default:
        if (keycode <= 0xFFFF) { // Try to obtain the key from it's "character"
            const SHORT vKey = VkKeyScanW(static_cast<WCHAR>(keycode));
            if (vKey > -1) {
                return LOBYTE(vKey);
            } else {
                return static_cast<BYTE>(keycode);
            }
        } else {
            ok = false;
            return 0;
        }
    }
}

short HotkeyUtil::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    short nMods = 0;
    if (modifiers & Qt::ShiftModifier)
        nMods |= MOD_SHIFT;
    if (modifiers & Qt::ControlModifier)
        nMods |= MOD_CONTROL;
    if (modifiers & Qt::AltModifier)
        nMods |= MOD_ALT;
    return nMods;
}

short HotkeyUtil::modifierVkToMod(DWORD key)
{
    switch (key) {
    case VK_LMENU:
    case VK_RMENU:
    case VK_MENU:
        return MOD_ALT;
    case VK_LCONTROL:
    case VK_RCONTROL:
    case VK_CONTROL:
        return MOD_CONTROL;
    case VK_LSHIFT:
    case VK_RSHIFT:
    case VK_SHIFT:
        return MOD_SHIFT;
    }
    return 0;
}

DWORD HotkeyUtil::escapeNumPad(DWORD key)
{
    if (key >= VK_NUMPAD0 && key <= VK_NUMPAD9) {
        return key - (VK_NUMPAD0 - 0x30); // 0x30 == 1
    }
    return key;
}

bool HotkeyUtil::isModifier(DWORD key)
{
    return key >= VK_LSHIFT && key <= VK_RMENU || key >= VK_SHIFT && key <= VK_MENU;
}
