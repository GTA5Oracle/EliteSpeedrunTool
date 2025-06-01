#include "GlobalData.h"
#include "LanguageUtil.h"
#include "LogUtil.h"
#include "MainWindow.h"
#include "UiAccessUtil.h"
#include "event/EventBus.h"
#include "event/observer/CmdObserver.h"

#include <QApplication>
#include <QIcon>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char* argv[])
{
    // 设置日志输出格式
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] [%{pid}] %{message}\n");
    // 日志
    qInstallMessageHandler(myMessageHandler);

    QApplication a(argc, argv);

    SetCurrentDirectoryW(a.applicationDirPath().toStdWString().c_str());

#ifndef QT_DEBUG
    if (ERROR_SUCCESS != uiAccessUtil->prepareForUIAccess()) {
        QMessageBox::critical(nullptr, QString(), QObject::tr("获取 UIAccess 权限失败，无法在全屏的游戏上显示内容！"));
    }
#endif

    globalData->init();
    LanguageUtil::applyLanguage();

    // 必须设置为 false。否则在没有顶层窗口或者隐藏主窗口后，如果有子窗口弹出等，若关闭该弹出的子窗口，就会导致主窗口也被关闭
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setWindowIcon(QIcon("://image/ic_icon.png"));
    QApplication::setOrganizationName("SkyD666");
    QApplication::setApplicationName("Elite Speedrun Tool");
    QApplication::setApplicationDisplayName(QObject::tr("精英速通工具"));
    QApplication::setApplicationVersion("11.9.1.70");
    qputenv("ApplicationVersionCode", "20250601");

    qApp->setStyle(globalData->styleName());

    if (QApplication::applicationVersion() != globalData->version()) {
        QMessageBox::warning(nullptr, QString(), QObject::tr("*** 应用版本变更，请认真检查各项配置（热键、外观、提示音等）是否需要重新设置！***"));
    }

    CmdObserver o;
    eventBus->addObserver(&o);
    MainWindow w;
    if (a.arguments().contains("-minimized")) {
        w.showMinimized();
    } else {
        w.show();
    }

    return a.exec();
}
