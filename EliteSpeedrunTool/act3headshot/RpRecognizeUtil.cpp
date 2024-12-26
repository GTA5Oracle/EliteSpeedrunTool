#include "RpRecognizeUtil.h"

#include "GlobalData.h"
#include "TimeUtil.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QPixmap>
#include <QProcess>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QScreen>
#include <QString>
#include <cstring>
#include <windows.h>
#include <winuser.h>

int RpRecognizeUtil::startRp = 0;

RpRecognizeUtil::RpRecognizeUtil(QObject* parent)
    : QObject { parent }
{
}

int RpRecognizeUtil::start(QWidget* widget, QPixmap& image)
{
    sendInput();
    TimeUtil::delay(500);

    startRp = rpRecognize(widget, image).first;
    return startRp;
}

int RpRecognizeUtil::stop(QWidget* widget, bool isHost, QPixmap& image, int& rp)
{
    sendInput();
    TimeUtil::delay(500);

    int currentRp = rpRecognize(widget, image).first;
    rp = currentRp;
    int duration = currentRp - startRp;

    int delta = 200; // 智障门
    delta += 25 * 2; // 天基炮
    if (isHost) {
        delta += 100; // 进隧道主力+100
        delta += 25 * 4; // 主力一共4个重甲
    } else {
        delta += 25 * 3; // 副手一共3个重甲
    }

    duration -= delta;

    // CoInitialize(NULL);
    // ISpVoice* pVoice = nullptr;
    // HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    // if (SUCCEEDED(hr)) {
    //     hr = pVoice->Speak(QString::number(duration / 25).toStdWString().c_str(), 0, NULL);
    //     pVoice->Release();
    //     pVoice = NULL;
    // }
    // CoUninitialize();
    return duration / 25;
}

QPair<int, int> RpRecognizeUtil::rpRecognize(QWidget* widget, QPixmap& image)
{
    auto file = QDir(QCoreApplication::applicationDirPath());
    file.mkpath("cache/rpregion");
    if (file.cd("cache/rpregion")) {
        if (file.entryInfoList(QStringList() << "*.*").size() > 10) {
            file.removeRecursively();
            file.cdUp();
            file.mkdir("rpregion");
            file.cd("rpregion");
        }
    }
    auto fileName = file.filePath(QString::number(QRandomGenerator::global()->bounded(0, INT_MAX)) + ".jpg");
    qDebug() << fileName;
    image = regionPixmap(widget, globalData->rpRect());
    image.save(fileName);
    return rpRecognize(fileName.replace("/", "\\"));
}

QPair<int, int> RpRecognizeUtil::rpRecognize(QString image)
{
    QProcess process;
    process.start("external/Tesseract-OCR/tesseract.exe",
        QStringList() << image
                      << "stdout"
                      << "-l" << "eng"
                      << "-c" << "tessedit_char_whitelist=1234567890/"
                      << "--oem" << "1" << "--psm" << "7");
    if (!process.waitForFinished()) {
        qDebug() << "Process failed to finish:" << process.errorString();
        return qMakePair(0, 0);
    }

    QString out = process.readAllStandardOutput().trimmed();
    static auto reg = QRegularExpression("[^0-9]");
    out.remove(reg);
    qDebug() << out << process.readAllStandardError();
    int len = out.size();
    int rp, nextRp;
    rp = out.left(len / 2).toInt();
    nextRp = out.right(len / 2).toInt();
    return qMakePair(rp, nextRp);
}

QPixmap RpRecognizeUtil::regionPixmap(QWidget* widget, QRect rect)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    return screen->grabWindow(0).copy(rect);
}

void RpRecognizeUtil::sendInput()
{
    INPUT inputs[1] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0x5A;
    inputs[0].ki.wScan = MapVirtualKey(0x5A, MAPVK_VK_TO_VSC);
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    TimeUtil::delay(50);

    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0x5A;
    inputs[0].ki.wScan = MapVirtualKey(0x5A, MAPVK_VK_TO_VSC);
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}
