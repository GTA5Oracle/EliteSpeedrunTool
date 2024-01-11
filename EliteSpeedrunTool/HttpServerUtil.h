#pragma once

#include <QGlobalStatic>
#include <QHttpServer>
#include <QObject>
#include <QWebSocketServer>

class HttpServerUtil : public QObject {
    Q_OBJECT

signals:
    void send(short headshotCount);

public:
    struct DataPackage {
        short headshotCount = 0;
        bool timerIsRunning = false;
        bool timerReset = false;
    };
    enum TimerState {
        Running = 0x02,
        Paused = 0x05,
        Zero = 0x01,
        Stopped = Paused | Zero,
        ZeroAndRunning = Running | Zero,
    };

    HttpServerUtil();
    ~HttpServerUtil();

    void startHttp();
    void stopHttp();

    void startTimer(bool isContinue, qint64 startTimestamp);
    void stopTimer(qint64 stoppedTime);
    void pauseTimer(qint64 pausedTimestamp);
    void zeroTimer();

    void sendNewData();

    void sendNewData(QJsonDocument json);

    void sendNewData(short headshotCount);

    void sendNewData(QWebSocket* webSocket);

    static QString getHttpServerDomain();

protected:
    void onNewConnection();

    void onCloseConnection();

    void socketDisconnected();

    QJsonDocument getHeadshotJson(DataPackage* data);

    QJsonDocument getTimerStateJson(
        TimerState state = HttpServerUtil::timerState,
        qint64 startTimestamp = HttpServerUtil::startTimestamp,
        qint64 pausedTimestamp = HttpServerUtil::pausedTimestamp);

public:
    static TimerState timerState;
    static qint64 startTimestamp;
    static qint64 pausedTimestamp;

private:
    bool started = false;

    static quint16 currentHttpPort;
    static quint16 currentWebsocketPort;

    QHttpServer* httpServer = nullptr;

    QWebSocketServer* webSocketServer = nullptr;

    QList<QWebSocket*> clients;

    DataPackage data;
};

class HttpServerController : public QObject {
    Q_OBJECT

public:
    static HttpServerController* instance();

    HttpServerController();
    ~HttpServerController();

    void start();

    void stop();

    void sendNewData(short headshotCount);
    void startOrContinueTimer(bool isContinue, qint64 startTimestamp);
    void stopTimer(qint64 stoppedTime);
    void pauseTimer(qint64 pausedTimestamp);
    void zeroTimer();
    void stopHttp();
    void initHttpServerUtil();

signals:
    void sendNewDataSignal(short headshotCount, QPrivateSignal);

    void startOrContinueTimerSignal(bool isContinue, qint64 startTimestamp, QPrivateSignal);
    void stopTimerSignal(qint64 stoppedTime, QPrivateSignal);
    void pauseTimerSignal(qint64 pausedTimestamp, QPrivateSignal);
    void zeroTimerSignal(QPrivateSignal);

    void stopHttpSignal(QPrivateSignal);

    void initHttpServerUtilSignal(QPrivateSignal);

private:
    QMutex mutex;

    bool started = false;

    HttpServerUtil* worker = nullptr;

    QThread* workerThread = nullptr;
};
