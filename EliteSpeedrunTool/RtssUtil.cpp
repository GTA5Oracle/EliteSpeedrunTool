#include "RtssUtil.h"
#include "GlobalData.h"
#include "RTSSSharedMemory.h"
#include <QApplication>
#include <QDebug>
#include <windows.h>

Q_GLOBAL_STATIC(RtssUtil, rtssUtilInstance)

RtssUtil::RtssUtil(QObject* parent)
    : QObject { parent }
{
    for (auto f : globalData->funcs()) {
        connect(globalData->displayInfoSubFunctions()[f],
            &DisplayInfoSubFunctionItem::rtssDisplayChanged, this,
            static_cast<bool (RtssUtil::*)(void)>(&RtssUtil::updateOsd));
    }

    updateTimer(0);
    updateAct3Headshot(0);

    if (globalData->rtssOverlay()) {
        initOverlay();
    }
    connect(globalData, &GlobalData::rtssOverlayChanged, this, [this]() {
        if (globalData->rtssOverlay()) {
            initOverlay();
        } else {
            releaseOverlay();
        }
    });

    connect(qApp, &QCoreApplication::aboutToQuit, this, [this]() {
        releaseOverlay();
    });
}

RtssUtil* RtssUtil::instance()
{
    return rtssUtilInstance;
}

void RtssUtil::initOverlay()
{
    if (overlayInited) {
        return;
    }
    overlayInited = true;
    updateOsd();
}

void RtssUtil::releaseOverlay()
{
    if (!overlayInited) {
        return;
    }
    overlayInited = false;

    HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "RTSSSharedMemoryV2");

    if (hMapFile) {
        LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

        if (pMem) {
            if ((pMem->dwSignature == rtssSignature) && (pMem->dwVersion >= 0x00020000)) {
                for (DWORD dwEntry = 1; dwEntry < pMem->dwOSDArrSize; dwEntry++) {
                    auto pEntry = (RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY)(
                        (LPBYTE)pMem + pMem->dwOSDArrOffset + dwEntry * pMem->dwOSDEntrySize);

                    if (!strcmp(pEntry->szOSDOwner, qAppName().toStdString().c_str())) {
                        memset(pEntry, 0, pMem->dwOSDEntrySize);
                        pMem->dwOSDFrame++;
                    }
                }
            }
            UnmapViewOfFile(pMapAddr);
        }
        CloseHandle(hMapFile);
    }
}

bool RtssUtil::updateTimer(unsigned long long deltaTime)
{
    auto subFunction = DisplayInfoSubFunction::Timer;
    subFunctionTextMap[subFunction] = getTimeString(deltaTime);
    if (!globalData->displayInfoSubFunctions()[subFunction]->rtssDisplay()) {
        return false;
    }
    return updateOsd();
}

bool RtssUtil::updateAct3Headshot(int headshot)
{
    auto subFunction = DisplayInfoSubFunction::Act3Headshot;
    subFunctionTextMap[subFunction] = globalData->displayInfoSubFunctions()[subFunction]
                                          ->rtssOsdText()
                                          .arg(headshot, 0);
    if (!globalData->displayInfoSubFunctions()[subFunction]->rtssDisplay()) {
        return false;
    }
    return updateOsd();
}

bool RtssUtil::check()
{
    if (!globalData->rtssOverlay()) {
        qInfo() << "RTSS is disabled";
        return false;
    }
    if (!overlayInited) {
        qInfo() << "RTSS is not inited";
        return false;
    }
    return true;
}

bool RtssUtil::updateOsd()
{
    QStringList stringList;
    for (auto i = subFunctionTextMap.constBegin(); i != subFunctionTextMap.constEnd(); i++) {
        if (globalData->displayInfoSubFunctions()[i.key()]->rtssDisplay()) {
            stringList << i.value();
        }
    }
    return updateOsd(stringList.join('\n'));
}

bool RtssUtil::updateOsd(QString text)
{
    if (!check()) {
        return false;
    }

    bool bResult = false;
    HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "RTSSSharedMemoryV2");

    if (hMapFile) {
        LPVOID pMapAddr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        LPRTSS_SHARED_MEMORY pMem = (LPRTSS_SHARED_MEMORY)pMapAddr;

        if (pMem) {
            if ((pMem->dwSignature == rtssSignature) && (pMem->dwVersion >= 0x00020000)) {
                for (DWORD dwPass = 0; dwPass < 2; dwPass++) {
                    // 1st pass : find previously captured OSD slot
                    // 2nd pass : otherwise find the first unused OSD slot and capture it
                    for (DWORD dwEntry = 1; dwEntry < pMem->dwOSDArrSize; dwEntry++) {
                        // allow primary OSD clients (i.e. EVGA Precision / MSI Afterburner) to use the first slot exclusively, so third party
                        // applications start scanning the slots from the second one
                        auto pEntry = (RTSS_SHARED_MEMORY::LPRTSS_SHARED_MEMORY_OSD_ENTRY)(
                            (LPBYTE)pMem + pMem->dwOSDArrOffset + dwEntry * pMem->dwOSDEntrySize);

                        if (dwPass) {
                            if (!strlen(pEntry->szOSDOwner)) {
                                strcpy_s(pEntry->szOSDOwner, sizeof(pEntry->szOSDOwner), qAppName().toStdString().c_str());
                            }
                        }

                        if (!strcmp(pEntry->szOSDOwner, qAppName().toStdString().c_str())) {
                            if (pMem->dwVersion >= 0x00020007) {
                                // use extended text slot for v2.7 and higher shared memory, it allows displaying 4096 symbols
                                // instead of 256 for regular text slot
                                if (pMem->dwVersion >= 0x0002000e) {
                                    // OSD locking is supported on v2.14 and higher shared memory
                                    DWORD dwBusy = _interlockedbittestandset(&pMem->dwBusy, 0);
                                    // bit 0 of this variable will be set if OSD is locked by renderer and cannot be refreshed
                                    // at the moment

                                    if (!dwBusy) {
                                        strncpy_s(pEntry->szOSDEx, sizeof(pEntry->szOSDEx),
                                            text.toStdString().c_str(), sizeof(pEntry->szOSDEx) - 1);
                                        pMem->dwBusy = 0;
                                    }
                                } else
                                    strncpy_s(pEntry->szOSDEx, sizeof(pEntry->szOSDEx),
                                        text.toStdString().c_str(), sizeof(pEntry->szOSDEx) - 1);

                            } else
                                strncpy_s(pEntry->szOSD, sizeof(pEntry->szOSD),
                                    text.toStdString().c_str(), sizeof(pEntry->szOSD) - 1);

                            pMem->dwOSDFrame++;
                            bResult = true;
                            break;
                        }
                    }

                    if (bResult) {
                        break;
                    }
                }
            }
            UnmapViewOfFile(pMapAddr);
        }
        CloseHandle(hMapFile);
    }
    return bResult;
}

QString RtssUtil::getTimeString(unsigned long long deltaTime)
{
    int m = deltaTime / 1000 / 60;
    int s = (deltaTime / 1000) % 60;
    int ms = (deltaTime % 1000) / 10;
    return globalData->displayInfoSubFunctions()[DisplayInfoSubFunction::Timer]
        ->rtssOsdText()
        .arg(m, 2, 10, QLatin1Char('0'))
        .arg(s, 2, 10, QLatin1Char('0'))
        .arg(ms, 2, 10, QLatin1Char('0'));
}
