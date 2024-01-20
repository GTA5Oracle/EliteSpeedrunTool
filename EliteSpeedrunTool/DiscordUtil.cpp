#include "DiscordUtil.h"
#include "GlobalData.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>

Q_GLOBAL_STATIC(DiscordUtil, discordUtilInstance)

DiscordUtil::DiscordUtil(QObject* parent)
    : QObject(parent)
{
    connect(globalData, &GlobalData::discordShowRichPresenceChanged, this, [this]() {
        if (globalData->discordShowRichPresence()) {
            activateDiscord();
        } else {
            deactivateDiscord();
        }
    });
}

DiscordUtil::~DiscordUtil()
{
    timer.stop();
}

DiscordUtil* DiscordUtil::instance()
{
    return discordUtilInstance;
}

bool DiscordUtil::initDiscord()
{
    if (inited) {
        return true;
    }
    discord::Core* core {};
    auto result = discord::Core::Create(1144642574037029045, DiscordCreateFlags_NoRequireDiscord, &core);
    state.core.reset(core);
    if (!state.core) {
        qWarning() << QString("Failed to instantiate discord core! (err %1)")
                          .arg(QString::number(static_cast<int>(result)));
        return false;
    }

    state.core->SetLogHook(
        discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) {
            qInfo() << QString("Discord Log(%1): %2")
                           .arg(QString::number(static_cast<uint32_t>(level)), message);
        });

    inited = true;

    if (inited && globalData->discordShowRichPresence()) {
        activateDiscord();
    }

    return true;
}

void DiscordUtil::setCallbackEnabled(bool enabled)
{
    enabled ? timer.start(500) : timer.stop();
}

void DiscordUtil::activateDiscord()
{
    initDiscord();

    syncSignalConnections = {
        connect(&timer, &QTimer::timeout, this, [this]() {
            if (state.core) {
                state.core->RunCallbacks();
                updateDiscordRpc();
            }
        })
    };

    //    deactivateDiscordRPC();
    setCallbackEnabled(true);

    activity.SetName("Elite Speedrun");
    activity.SetState(currentMission.toUtf8().constData());
    activity.GetTimestamps().SetStart(QDateTime::currentSecsSinceEpoch());
    activity.GetAssets().SetLargeImage("icon");
    activity.GetAssets().SetLargeText("Elite");
    activity.GetAssets().SetSmallImage("gtav");
    activity.GetAssets().SetSmallText("GTAV");
    activity.SetType(discord::ActivityType::Playing);

    updateDiscordRpc();
}

void DiscordUtil::deactivateDiscord()
{
    if (!state.core) {
        return;
    }
    state.core->ActivityManager().ClearActivity([this](discord::Result result) {
        if (result == discord::Result::Ok) {
            qInfo("Discord: ClearActivity success!");
            for (const QMetaObject::Connection& connection : syncSignalConnections) {
                disconnect(connection);
            }
            setCallbackEnabled(false);
            state.core.reset();
            inited = false;
            emit deactivated(QPrivateSignal());
        } else {
            qInfo("Discord: ClearActivity failed!");
        }
    });
}

void DiscordUtil::updateDiscordRpc()
{
    if (!state.core) {
        return;
    }
    activity.SetState(currentMission.toUtf8().constData());

    state.core->ActivityManager().UpdateActivity(activity, [this](discord::Result result) {
        if (result != discord::Result::Ok) {
            qInfo("Discord: UpdateActivity failed!");
        }
    });
}

void DiscordUtil::setCurrentMission(QString mission)
{
    this->currentMission = mission;
}
