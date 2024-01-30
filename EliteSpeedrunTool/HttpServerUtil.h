#pragma once

#include <QGlobalStatic>
#include <QHttpServer>
#include <QObject>
#include <QWebSocketServer>

class HttpServerUtil : public QObject {
    Q_OBJECT

signals:
    void send(QString missionData);

public:
    struct DataPackage {
        QString missionData = "";
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
    enum AutoTimerState {
        AutoTimerRunning = 0x02,
        AutoTimerPaused = 0x03,
        AutoTimerZero = 0x01,
    };

    HttpServerUtil();
    ~HttpServerUtil();

    void startHttp();
    void stopHttp();

    void startTimer(bool isContinue, qint64 startTimestamp);
    void stopTimer(qint64 stoppedTime);
    void pauseTimer(qint64 pausedTimestamp);
    void zeroTimer();

    // 传入计时器的时间，而不是现实的时间戳
    void startAutoTimer(qint64 initTimestamp);
    void pauseAutoTimer(qint64 initTimestamp);

    void sendNewData();

    void sendNewData(QJsonDocument json);

    void sendNewData(QString missionData);

    void sendNewData(QWebSocket* webSocket);

    static QString getHttpServerDomain();

protected:
    void onNewConnection();

    void onCloseConnection();

    void socketDisconnected();

    QJsonDocument getMissionDataJson(DataPackage* data);

    QJsonDocument getTimerStateJson(
        TimerState state = HttpServerUtil::timerState,
        qint64 startTimestamp = HttpServerUtil::startTimestamp,
        qint64 pausedTimestamp = HttpServerUtil::pausedTimestamp);

    QJsonDocument getAutoTimerStateJson(
        AutoTimerState state = HttpServerUtil::autoTimerState,
        qint64 startTimestamp = HttpServerUtil::startAutoTimestamp,
        qint64 initAutoTimestamp = HttpServerUtil ::initAutoTimestamp,
        qint64 pausedTimestamp = HttpServerUtil::pausedAutoTimestamp);

public:
    static TimerState timerState;
    static qint64 startTimestamp;
    static qint64 pausedTimestamp;

    static AutoTimerState autoTimerState;
    static qint64 initAutoTimestamp;
    static qint64 startAutoTimestamp;
    static qint64 pausedAutoTimestamp;

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

    void sendNewData(QString missionData);

    void startOrContinueTimer(bool isContinue, qint64 startTimestamp);
    void stopTimer(qint64 stoppedTime);
    void pauseTimer(qint64 pausedTimestamp);
    void zeroTimer();

    void startAutoTimer(qint64 startTimestamp);
    void pauseAutoTimer(qint64 pausedTimestamp);

    void stopHttp();
    void initHttpServerUtil();

    bool isStarted() const;

signals:
    void sendNewDataSignal(QString missionData, QPrivateSignal);

    void startOrContinueTimerSignal(bool isContinue, qint64 startTimestamp, QPrivateSignal);
    void stopTimerSignal(qint64 stoppedTime, QPrivateSignal);
    void pauseTimerSignal(qint64 pausedTimestamp, QPrivateSignal);
    void zeroTimerSignal(QPrivateSignal);

    void startAutoTimerSignal(qint64 startTimestamp, QPrivateSignal);
    void pauseAutoTimerSignal(qint64 pausedTimestamp, QPrivateSignal);

    void stopHttpSignal(QPrivateSignal);

    void initHttpServerUtilSignal(QPrivateSignal);

private:
    QMutex mutex;

    bool started = false;

    HttpServerUtil* worker = nullptr;

    QThread* workerThread = nullptr;
};
