#include "CmdEventHelper.h"
#include <QMetaObject>
#include <QMetaType>

Q_GLOBAL_STATIC(CmdEventHelper, cmdEventHelperInstance)

CmdEventHelper::CmdEventHelper(QObject* parent)
    : QObject { parent }
{
}

CmdEventHelper* CmdEventHelper::instance()
{
    return cmdEventHelperInstance;
}

QString CmdEventHelper::eventDisplayName(QString name)
{
    auto iter = eventNameMap.find(name);
    if (iter == eventNameMap.end()) {
        return "Unknown";
    }
    return iter.value();
}

const QMap<QString, QString>& CmdEventHelper::getEventNameMap() const
{
    return eventNameMap;
}
