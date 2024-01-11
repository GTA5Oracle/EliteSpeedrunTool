#include "HttpServerUtil.h"
#include "GlobalData.h"
#include "LogUtil.h"

#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>

Q_GLOBAL_STATIC(HttpServerController, controllerInstance)

quint16 HttpServerUtil::currentHttpPort = 0;
quint16 HttpServerUtil::currentWebsocketPort = 0;
HttpServerUtil::TimerState HttpServerUtil::timerState = TimerState::Stopped;
qint64 HttpServerUtil::startTimestamp = 0;
qint64 HttpServerUtil::pausedTimestamp = 0;

HttpServerUtil::HttpServerUtil()
{
}

HttpServerUtil::~HttpServerUtil()
{
}

void HttpServerUtil::startHttp()
{
    if (started) {
        return;
    }
    started = true;
    httpServer = new QHttpServer(this);
    webSocketServer = new QWebSocketServer("Act3 Speedrun Tool", QWebSocketServer::NonSecureMode, this);
    httpServer->route("/favicon.ico", [](QHttpServerResponder&& responder) {
        auto favicon = new QFile("./html/favicon.ico");
        responder.write(favicon,
            { qMakePair("Content-Type", "image/x-icon"),
                qMakePair("Cache-Control", "max-age=7200") });
    });
    httpServer->route("/displayInfo", [](QHttpServerResponder&& responder) {
        auto htmlFile = new QFile("./html/DisplayInfo.html");
        responder.write(htmlFile, "text/html");
    });
    httpServer->route("/reconnecting-websocket.min.js", [](QHttpServerResponder&& responder) {
        auto jsFile = new QFile("./html/reconnecting-websocket.min.js");
        responder.write(jsFile, "text/javascript");
    });
    httpServer->route("/displayInfo.js", [](QHttpServerResponder&& responder) {
        auto jsFile = new QFile("./html/DisplayInfo.js");
        responder.write(jsFile, "text/javascript");
    });
    httpServer->route("/displayInfo.css", [](QHttpServerResponder&& responder) {
        auto cssFile = new QFile("./html/DisplayInfo.css");
        responder.write(cssFile, "text/css");
    });
    currentHttpPort = httpServer->listen(QHostAddress::Any, globalData->serverHttpPort());
    qInfo("Http server listened on: %d", currentHttpPort);
    if (webSocketServer->listen(QHostAddress::Any, globalData->serverWebsocketPort())) {
        currentWebsocketPort = globalData->serverWebsocketPort();
        connect(webSocketServer, &QWebSocketServer::newConnection, this, &HttpServerUtil::onNewConnection);
        qInfo("WebSocket server listened on: %d", currentWebsocketPort);
    } else {
        qCritical("WebSocket server listen failed, port is %d", globalData->serverWebsocketPort());
    }
}

void HttpServerUtil::stopHttp()
{
    webSocketServer->close();

    httpServer->deleteLater();
    webSocketServer->deleteLater();

    qInfo("Http server stopped");
    qInfo("WebSocket server stopped");

    QThread::currentThread()->quit();
}

void HttpServerUtil::startTimer(bool isContinue, qint64 startTimestamp)
{
    HttpServerUtil::startTimestamp = startTimestamp;
    timerState = TimerState(TimerState::Running | (isContinue ? 0 : TimerState::Zero));
    sendNewData(getTimerStateJson());
}

void HttpServerUtil::stopTimer(qint64 stoppedTime)
{
    timerState = TimerState::Stopped;
    pausedTimestamp = stoppedTime;
    sendNewData(getTimerStateJson());
}

void HttpServerUtil::pauseTimer(qint64 pausedTimestamp)
{
    HttpServerUtil::pausedTimestamp = pausedTimestamp;
    timerState = TimerState::Paused;
    sendNewData(getTimerStateJson());
}

void HttpServerUtil::zeroTimer()
{
    timerState = TimerState::Zero;
    sendNewData(getTimerStateJson());
}

void HttpServerUtil::sendNewData()
{
    for (auto c : clients) {
        c->sendTextMessage(getHeadshotJson(&data).toJson(QJsonDocument::Compact));
    }
}

void HttpServerUtil::sendNewData(QJsonDocument json)
{
    QString jsonString = json.toJson(QJsonDocument::Compact);
    for (auto c : clients) {
        c->sendTextMessage(jsonString);
    }
}

void HttpServerUtil::sendNewData(short headshotCount)
{
    data.headshotCount = headshotCount;
    sendNewData();
}

void HttpServerUtil::sendNewData(QWebSocket* webSocket)
{
    webSocket->sendTextMessage(getHeadshotJson(&data).toJson(QJsonDocument::Compact));
    webSocket->sendTextMessage(getTimerStateJson().toJson(QJsonDocument::Compact));
}

QString HttpServerUtil::getHttpServerDomain()
{
    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
        QString addressString = address.toString();
        if (address.protocol() == QAbstractSocket::IPv4Protocol
            && address != localhost
            && addressString.section(".", -1, -1) != "1") {
            return QString("http://%1:%2/displayInfo").arg(addressString, QString::number(currentHttpPort));
        }
    }
    return "";
}

void HttpServerUtil::onNewConnection()
{
    QWebSocket* pSocket = webSocketServer->nextPendingConnection();

    //    connect(pSocket, &QWebSocket::textMessageReceived, this, &EchoServer::processTextMessage);
    //    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &HttpServerUtil::socketDisconnected);

    clients << pSocket;

    sendNewData(pSocket);

    qInfo() << "Socket connected:" << pSocket->peerAddress().toString();
}

void HttpServerUtil::onCloseConnection()
{
}

void HttpServerUtil::socketDisconnected()
{
    QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());
    if (pClient) {
        clients.removeAll(pClient);
        qInfo() << "Socket disconnected:" << pClient->peerAddress().toString();
        pClient->deleteLater();
    }
}

QJsonDocument HttpServerUtil::getHeadshotJson(DataPackage* data)
{
    QJsonObject object {
        { "headshotCount", data->headshotCount },
    };
    return QJsonDocument(object);
}

QJsonDocument HttpServerUtil::getTimerStateJson(
    TimerState state,
    qint64 startTimestamp,
    qint64 pausedTimestamp)
{
    QJsonObject object {
        { "timerState", state },
        { "starttedTimestamp", startTimestamp },
        { "pausedTimestamp", pausedTimestamp },
        { "serverTimestamp", QDateTime::currentDateTime().toMSecsSinceEpoch() },
    };
    return QJsonDocument(object);
}

HttpServerController* HttpServerController::instance()
{
    return controllerInstance;
}

HttpServerController::HttpServerController()
{
}

HttpServerController::~HttpServerController()
{
    stop();
}

void HttpServerController::start()
{
    QMutexLocker locker(&mutex);
    if (started) {
        return;
    }
    worker = new HttpServerUtil;
    workerThread = new QThread;
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &HttpServerController::sendNewDataSignal, worker, QOverload<short>::of(&HttpServerUtil::sendNewData));
    connect(this, &HttpServerController::startOrContinueTimerSignal, worker, QOverload<bool, qint64>::of(&HttpServerUtil::startTimer));
    connect(this, &HttpServerController::stopTimerSignal, worker, &HttpServerUtil::stopTimer);
    connect(this, &HttpServerController::pauseTimerSignal, worker, &HttpServerUtil::pauseTimer);
    connect(this, &HttpServerController::zeroTimerSignal, worker, &HttpServerUtil::zeroTimer);
    connect(this, &HttpServerController::stopHttpSignal, worker, &HttpServerUtil::stopHttp);
    connect(this, &HttpServerController::initHttpServerUtilSignal, worker, &HttpServerUtil::startHttp);
    workerThread->start();
    emit initHttpServerUtilSignal(QPrivateSignal());
    started = true;
}

void HttpServerController::stop()
{
    QMutexLocker locker(&mutex);
    if (!started) {
        return;
    }
    emit stopHttpSignal(QPrivateSignal());
    workerThread->wait();
    started = false;
}

void HttpServerController::sendNewData(short headshotCount)
{
    emit sendNewDataSignal(headshotCount, QPrivateSignal());
}

void HttpServerController::startOrContinueTimer(bool isContinue, qint64 startTimestamp)
{
    if (!started) {
        HttpServerUtil::timerState = HttpServerUtil::TimerState(HttpServerUtil::TimerState::Running
            | (isContinue ? 0 : HttpServerUtil::TimerState::Zero));
        HttpServerUtil::startTimestamp = startTimestamp;
    }
    emit startOrContinueTimerSignal(isContinue, startTimestamp, QPrivateSignal());
}

void HttpServerController::stopTimer(qint64 stoppedTime)
{
    if (!started) {
        HttpServerUtil::timerState = HttpServerUtil::TimerState::Stopped;
    }
    emit stopTimerSignal(stoppedTime, QPrivateSignal());
}

void HttpServerController::pauseTimer(qint64 pausedTimestamp)
{
    if (!started) {
        HttpServerUtil::timerState = HttpServerUtil::TimerState::Paused;
        HttpServerUtil::pausedTimestamp = pausedTimestamp;
    }
    emit pauseTimerSignal(pausedTimestamp, QPrivateSignal());
}

void HttpServerController::zeroTimer()
{
    if (!started) {
        HttpServerUtil::timerState = HttpServerUtil::TimerState::Zero;
    }
    emit zeroTimerSignal(QPrivateSignal());
}

void HttpServerController::stopHttp()
{
    emit stopHttpSignal(QPrivateSignal());
}

void HttpServerController::initHttpServerUtil()
{
    emit initHttpServerUtilSignal(QPrivateSignal());
}
