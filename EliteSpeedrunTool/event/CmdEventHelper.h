#pragma once

#include "Event.h"

#include <QMap>
#include <QMetaObject>
#include <QMetaType>
#include <QObject>

#define cmdEventHelper (CmdEventHelper::instance())

class CmdEventHelper : public QObject {
    Q_OBJECT
public:
    explicit CmdEventHelper(QObject* parent = nullptr);

    static CmdEventHelper* instance();

    QString eventDisplayName(QString name);

    const QMap<QString, QString>& getEventNameMap() const;

private:
    QMap<QString, QString> eventNameMap = {
        qMakePair(QMetaType::fromType<StartEvent>().metaObject()->className(), tr("启动")),
        qMakePair(QMetaType::fromType<QuitEvent>().metaObject()->className(), tr("退出")),
        qMakePair(QMetaType::fromType<CloseGameEvent>().metaObject()->className(), tr("结束游戏")),
    };
};
