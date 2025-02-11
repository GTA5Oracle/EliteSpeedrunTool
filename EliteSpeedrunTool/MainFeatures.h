#pragma once

#include <QObject>
#include <windows.h>

#define mainFeatures (MainFeatures::instance())

class MainFeatures : public QObject {
    Q_OBJECT
public:
    explicit MainFeatures(QObject* parent = nullptr);

    static MainFeatures* instance();

    void terminateGta();

    void applyWindowDisplayAffinity(QWidget* widget);

private:
    bool ntTerminateProcess(DWORD pid);
    bool terminateProcess(DWORD pid);
signals:
};
