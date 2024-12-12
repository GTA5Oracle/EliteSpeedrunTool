#include "HotkeyUtil.h"
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
    nativeKeycode = hotkeyUtil->escapeNumPad(hotkeyUtil->nativeKeycode(hotkey.key(), nativeKeycodeOk));
    nativeModifiers = hotkeyUtil->nativeModifiers(hotkey.keyboardModifiers());
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

HotkeyUtil::HotkeyUtil(QWidget* parent)
    : QWidget { parent }
{
    qInfo() << "HotkeyUtil initializing...";
    registerRawInputDevice();
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
        modifiers |= modifierVkToMod(key);
        return;
    }
    key = escapeNumPad(key);
    for (const auto& hotkeySeq : hotkeys) {
        // qDebug() << hotkeySeq->nativeModifiers << modifiers << hotkeySeq->nativeKeycodeOk << hotkeySeq->nativeKeycode << key;
        if (hotkeySeq->nativeModifiers != modifiers || !hotkeySeq->nativeKeycodeOk || hotkeySeq->nativeKeycode != key) {
            continue;
        }
        hotkeySeq->emitSignal();
    }
}

void HotkeyUtil::keyUp(DWORD key)
{
    if (isModifier(key)) {
        modifiers &= ~modifierVkToMod(key);
        return;
    }
    // key = escapeNumPad(key);
}

bool HotkeyUtil::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
        MSG* msg = static_cast<MSG*>(message);

        if (msg->message == WM_INPUT) {
            processRawInput(msg->lParam);
        }
    }
    return QWidget::nativeEvent(eventType, message, result);
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
    UINT dwSize = 0;
    // Get the input data size
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

    // Allocate buffer
    LPBYTE lpb = new BYTE[dwSize];
    if (!lpb) {
        return;
    }

    // Get the input data
    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
        qCritical() << "GetRawInputData does not return correct size!";
    }

    // Parse RAWINPUT
    RAWINPUT* raw = (RAWINPUT*)lpb;
    if (raw->header.dwType == RIM_TYPEKEYBOARD) {
        RAWKEYBOARD keyboard = raw->data.keyboard;
        UINT vKey = keyboard.VKey; // Virtual key code
        // UINT scanCode = keyboard.MakeCode; // Scan code
        UINT flags = keyboard.Flags; // Key Down / Key Up

        if (flags & RI_KEY_BREAK) {
            keyUp(vKey);
        } else {
            keyDown(vKey);
        }
    }

    delete[] lpb;
}

quint32 HotkeyUtil::nativeKeycode(Qt::Key keycode, bool& ok)
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

quint32 HotkeyUtil::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    quint32 nMods = 0;
    if (modifiers & Qt::ShiftModifier)
        nMods |= MOD_SHIFT;
    if (modifiers & Qt::ControlModifier)
        nMods |= MOD_CONTROL;
    if (modifiers & Qt::AltModifier)
        nMods |= MOD_ALT;
    if (modifiers & Qt::MetaModifier)
        nMods |= MOD_WIN;
    return nMods;
}

quint32 HotkeyUtil::modifierVkToMod(DWORD key)
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
    case VK_LWIN:
    case VK_RWIN:
        return MOD_ALT;
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
    return key >= VK_LSHIFT && key <= VK_RMENU
        || key >= VK_SHIFT && key <= VK_MENU
        || key >= VK_LWIN && key <= VK_RWIN;
}
