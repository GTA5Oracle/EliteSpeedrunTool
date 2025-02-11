#pragma once

#include <QCursor>
#include <QObject>

#define imageUtil (ImageUtil::instance())

class ImageUtil : public QObject {
    Q_OBJECT
public:
    explicit ImageUtil(QObject* parent = nullptr);

    static ImageUtil* instance();

    QCursor cursorFromCurFile(QString curPath);

signals:
};
