#pragma once

#include <QObject>
#include <Wbemidl.h>
#include <setupapi.h>
#include <windows.h>

#define networkAdapterUtil (NetworkAdapterUtil::instance())

struct NetworkAdapter {
    QString name;
    QString deviceId;
    NetworkAdapter(QString name, QString deviceId)
    {
        this->name = name;
        this->deviceId = deviceId;
    }
};

class NetworkAdapterUtil : public QObject {
    Q_OBJECT
public:
    explicit NetworkAdapterUtil(QObject* parent = nullptr);

    static NetworkAdapterUtil* instance();

    QList<NetworkAdapter> networkAdapters();

    QList<QString> adapterNames(QList<QString> deviceIds);

    bool setNetworkAdaptersEnabled(QList<QString> deviceIds, bool enabled);

private:
    void initNetworkAdapterGuid();

    bool setSingleNetworkAdapterEnabled(HDEVINFO deviceInfoSet, QString deviceId, bool enabled);

    GUID networkAdapterGuid;

signals:
};
