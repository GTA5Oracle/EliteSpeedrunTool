#pragma once
#include <QObject>
#include <windows.h>

#define uiAccessUtil (UiAccessUtil::instance())

class UiAccessUtil : public QObject {
    Q_OBJECT
public:
    explicit UiAccessUtil(QObject* parent = nullptr);

    static UiAccessUtil* instance();

    DWORD prepareForUIAccess();

    BOOL checkForUIAccess(DWORD* pdwErr, DWORD* pfUIAccess);

    DWORD duplicateWinloginToken(DWORD dwSessionId, DWORD dwDesiredAccess, PHANDLE phToken);

    DWORD createUIAccessToken(PHANDLE phToken);
signals:
};
