#pragma once

#include <QKeySequence>
#include <QList>
#include <QWidget >
#include <dinput.h>
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

    quint32 nativeKeycode(Qt::Key keycode, bool& ok);
    quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
    quint32 modifierVkToMod(DWORD key);
    DWORD escapeNumPad(DWORD key);

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private:
    int modifiers;

    QList<QHotkey*> hotkeys;

    bool isModifier(DWORD key);
};
