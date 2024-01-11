#pragma once
#include "discord.h"
#include <QEventLoop>
#include <QMutex>
#include <QObject>
#include <QTimer>
#include <memory>

#define discordUtil (DiscordUtil::instance())

struct DiscordState {
    discord::User currentUser;

    std::unique_ptr<discord::Core> core;
};

class DiscordUtil : public QObject {
    Q_OBJECT

public:
    DiscordUtil(QObject* parent = nullptr);
    ~DiscordUtil();

    static DiscordUtil* instance();

    bool initDiscord();

    void setCallbackEnabled(bool enabled);

    void activateDiscord();

    void deactivateDiscord();

    void updateDiscordRpc();

    void setSpeedrunTime(qint64 m, qint64 s);

    void setHeadshotCount(short newHeadshotCount);

private:
    bool inited = false;

    QMutex mutex;

    QTimer timer;

    DiscordState state {};

    discord::Activity activity = discord::Activity();

    QList<QMetaObject::Connection> syncSignalConnections;

    QString speedrunTime = "Time: 00:00";
    QString headshotCount = "Head: 0";

signals:
    void deactivated(QPrivateSignal);
};
