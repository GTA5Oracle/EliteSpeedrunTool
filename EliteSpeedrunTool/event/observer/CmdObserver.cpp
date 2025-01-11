#include "CmdObserver.h"
#include "GlobalData.h"

CmdObserver::CmdObserver(QObject* parent)
    : EventObserver { parent }
{
}

void CmdObserver::onEvent(Event* event)
{
    const auto& map = globalData->eventCmdMap();
    auto className = event->metaObject()->className();
    if (map.contains(className)) {
        auto cmd = map[className];
        if (!cmd.isEmpty()) {
            QString realCmd = "powershell \"" + cmd.replace("\"", "\\\"") + "\"";
            int ret = WinExec(realCmd.toLocal8Bit().toStdString().c_str(), SW_HIDE);
            qInfo() << "Command" << cmd << "ret: " << ret;
        }
    }
}
