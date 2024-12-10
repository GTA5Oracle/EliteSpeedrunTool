#pragma once

#include <QKeySequence>
#include <QList>
#include <QObject>
#include <windows.h>

#define hotkeyUtil (HotkeyUtil::instance())

class QHotkey : public QObject {
    Q_OBJECT
public:
    QHotkey(QKeySequence keySeq, bool autoRegister, QObject* parent = nullptr);
    ~QHotkey();
    QKeySequence keySeq;

    void setRegistered(bool registered);

    void emitSignal();

    int nativeKeycode;
    int nativeModifiers;
    bool nativeKeycodeOk = false;
signals:
    void activated();
};

class HotkeyUtil : public QObject {
    Q_OBJECT
public:
    explicit HotkeyUtil(QObject* parent = nullptr);

    ~HotkeyUtil();

    static HotkeyUtil* instance();

    void registerHotkey(QHotkey* ks);
    void unregisterHotkey(QHotkey* ks);

    void keyDown(DWORD key);
    void keyUp(DWORD key);

    quint32 nativeKeycode(Qt::Key keycode, bool& ok);
    quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
    quint32 vkToMod(DWORD key);
    DWORD escapeNumPad(DWORD key);

private:
    HHOOK hhkKeyboard = nullptr;

    int modifiers;

    QList<QHotkey*> hotkeys;

    bool isModifier(DWORD key);
};
