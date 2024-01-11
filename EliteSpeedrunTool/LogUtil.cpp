#include "LogUtil.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>

#define QT_MESSAGELOGCONTEXT

Q_GLOBAL_STATIC(LogController, logControllerInstance)

QString LogUtil::logFileName = "Log - " + QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") + ".log";
bool LogUtil::firstTime = true;
bool LogController::appIsQuiting = false;

LogUtil::LogUtil()
{
}

QString LogUtil::getLogDir()
{
    return QCoreApplication::applicationDirPath() + "/Logs";
}

QString LogUtil::getLogFilePath()
{
    return getLogDir() + "/" + logFileName;
}

void LogUtil::initLog()
{
    QDir logDir(getLogDir());
    if (!logDir.exists()) {
        logDir.mkpath(getLogDir());
    }
    if (firstTime) {
        if (logDir.entryInfoList(QStringList() << "*.log").size() > 10) {
            logDir.removeRecursively();
            logDir.mkpath(getLogDir());
        }
        firstTime = false;
    }
    logFile = new QFile(getLogFilePath(), this);
    logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

void LogUtil::closeLog()
{
    if (logFile) {
        logFile->close();
        delete logFile;
        logFile = nullptr;
    }
}

void LogUtil::addLog(const QString& msg)
{
    QMutexLocker locker(&mutex);
    QTextStream textStream(logFile);

    textStream << msg;
    textStream.flush();

#ifdef QT_DEBUG // 只在Debug模式下在控制台输出
    QTextStream consoleOut(stdout);
    consoleOut << msg;
#endif
}

LogController* LogController::instance()
{
    return logControllerInstance;
}

LogController::LogController()
{
    connect(qApp, &QCoreApplication::aboutToQuit, this, [=]() {
        appIsQuiting = true;
    });
    open();
}

LogController::~LogController()
{
    innerCloseLog();
}

void LogController::open()
{
    QMutexLocker locker(&mutex);
    if (isOpened) {
        return;
    }
    worker = new LogUtil;
    workerThread = new QThread(this);
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &LogController::addLogSignal, worker, &LogUtil::addLog);
    connect(this, &LogController::closeLog, worker, &LogUtil::closeLog);
    connect(this, &LogController::initLog, worker, &LogUtil::initLog);
    workerThread->start();
    emit initLog();
    isOpened = true;
}

void LogController::innerCloseLog()
{
    QMutexLocker locker(&mutex);
    if (!isOpened) {
        return;
    }
    emit closeLog();
    workerThread->quit();
    workerThread->wait();
    workerThread->deleteLater();
    isOpened = false;
}

void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    if (!LogController::appIsQuiting) {
        emit logController->addLogSignal(qFormatLogMessage(type, context, msg));
    }
}
