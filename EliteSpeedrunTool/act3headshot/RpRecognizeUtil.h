#pragma once

#include <QObject>
#include <QPair>

class RpRecognizeUtil : public QObject {
    Q_OBJECT
public:
    explicit RpRecognizeUtil(QObject* parent = nullptr);

    static int start(QWidget* widget, QPixmap& image);

    static int stop(QWidget* widget, bool isHost, QPixmap& image, int& rp);

    static QPair<int, int> rpRecognize(QWidget* widget, QPixmap& image);

    static QPair<int, int> rpRecognize(QString image);

    static QPixmap regionPixmap(QWidget* widget, QRect region);

    static int startRp;

    static void sendInput();

signals:
};
