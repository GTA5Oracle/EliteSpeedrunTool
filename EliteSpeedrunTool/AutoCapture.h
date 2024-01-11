#pragma once
#include "MemoryUtil.h"
#include <QObject>

// #define AutoCapture (AutoCapture::instance());

class AutoCapture : public QObject {
    Q_OBJECT
public:
    AutoCapture();

    int Show_time;
    int show_extTime;
    int show_headshot;
    int show_headshot_host;
    int show_headshot_client_1;
    int show_kill;
    float show_vehicleDmg;
    float show_rashDmg;

    int Show_final_time;
    int show_final_extTime;
    int show_final_headshot;
    int show_final_kill;
    int show_final_vehicleDmg;
    int show_final_rashDmg;
};
