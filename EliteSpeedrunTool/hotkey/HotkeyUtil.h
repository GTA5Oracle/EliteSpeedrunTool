#pragma once

#include "HotkeyRedirector.h"
#include <QKeySequence>
#include <QList>
#include <QTimer>
#include <QWidget >
#include <dinput.h>
#include <windows.h>

#define hotkeyUtil (HotkeyUtil::instance())

class QHotkey : public QObject {
    Q_OBJECT
public:
    explicit QHotkey(QKeySequence keySeq, bool autoRegister, QObject* parent = nullptr);
    ~QHotkey();
    QKeySequence keySeq;

    void setRegistered(bool registered);

    void emitSignal();

    short nativeKeycode;
    short nativeModifiers;
    bool nativeKeycodeOk = false;
    bool isPressed = false;
signals:
    void activated();
};

class QHotkeyMap : public QHotkey {
    Q_OBJECT
public:
    explicit QHotkeyMap(QKeySequence keySeq, QObject* parent = nullptr);
    QList<int> nativeModifiers;
    void sendInput();

private:
    INPUT downInput = { 0 };
    INPUT upInput = { 0 };
};

class HotkeyUtil : public QWidget {
    Q_OBJECT
public:
    explicit HotkeyUtil(QWidget* parent = nullptr);

    static HotkeyUtil* instance();

    void registerHotkey(QHotkey* ks);
    void unregisterHotkey(QHotkey* ks);

    void keyDown(DWORD key);
    void keyUp(DWORD key);

    bool registerRawInputDevice();
    void processRawInput(LPARAM lParam);

    static short nativeKeycode(Qt::Key keycode, bool& ok);
    static short nativeModifiers(Qt::KeyboardModifiers modifiers);
    static short modifierVkToMod(DWORD key);
    static DWORD escapeNumPad(DWORD key);

    void setHotkeyMapEnabled(bool newHotkeyMapEnabled);

    bool isRegisteredInSystem(QKeyCombination key);

    void initLowLevelKeyboardHook();

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
    void closeEvent(QCloseEvent* event) override;

    void initHotkeyMaps();

    void installKeyboardHook();
    void uninstallKeyboardHook();
    static HHOOK keyboardHook;
    static LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    short modifiers;

    QList<QHotkey*> hotkeys;

    bool hotkeyMapEnabled = true;
    QList<QHotkeyMap*> hotkeyMaps = {};

    bool isModifier(DWORD key);

    HotkeyRedirector* hotkeyRedirector = new HotkeyRedirector(nullptr);

    QTimer reinstallKeyboardHookTimer = QTimer(this);
};
