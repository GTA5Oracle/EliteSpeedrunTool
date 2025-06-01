#include <QObject>
#include <QString>

#pragma once

class FirewallRule : public QObject {
public:
    FirewallRule(QString path, int direction, int protocol)
    {
        this->path = path;
        this->direction = direction;
        this->protocol = protocol;
    }

    QString path;
    int direction;
    int protocol;

    bool operator==(const FirewallRule& other)
    {
        return path == other.path && direction == other.direction && protocol == other.protocol;
    }
};
