#pragma once

#include <QDialog>
#include <windows.h>

class TimeUtil : public QDialog {
    Q_OBJECT
public:
    TimeUtil();

    static BOOLEAN nanosleep(LONGLONG ns);

    static long long currentTimestamp();

    static void delay(long long ms);
};
