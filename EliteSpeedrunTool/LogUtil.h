#include <QFile>
#include <QMutex>
#include <QString>
#include <QThread>
#include <QtLogging>

#pragma once

void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

class LogUtil : public QObject {
    Q_OBJECT

public:
    LogUtil();

    static QString getLogDir();

    static QString getLogFilePath();

    void initLog();

    void closeLog();

    void addLog(const QString& msg);

private:
    QMutex mutex;
    QFile* logFile = nullptr;
    static bool firstTime;
    static QString logFileName;
};

class LogController : public QObject {
    Q_OBJECT

public:
    static LogController* instance();

    LogController();
    ~LogController();
    static bool appIsQuiting;

signals:
    void initLog();
    void closeLog();
    void addLogSignal(const QString& msg);

private:
    QMutex mutex;

    bool isOpened = false;

    LogUtil* worker = nullptr;

    QThread* workerThread = nullptr;

    void open();
    void innerCloseLog();
};

#define logController (LogController::instance())
