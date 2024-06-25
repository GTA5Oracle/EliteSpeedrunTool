#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <windows.h>

#define memoryUtil (MemoryUtil::instance())

class MemoryUtil : public QObject {
    Q_OBJECT
public:
    explicit MemoryUtil();

    static MemoryUtil* instance();

    DWORD64 globalPtr = 0;
    DWORD64 missionPtr = 0;

    const static int globalMissionHash = 4718592 + 127178; // 任务HASH
    const static int globalSummaryTime = 2685444 + 6489; // 之前所有阶段耗费的时间总和
    const static int globalPrisonBreakSummaryTime = 2685444 + 6500; // 之前所有阶段耗费的时间总和
    // 变量的签名
    const static int localInMissionCanControl = 31694; // 是不是在任务中并且能够操控
    const static int localTime = 19746 + 987; // 任务每一段的时间
    const static int localPrisonBreakTime = 19746 + 990; // 越狱的逃离时间
    const static int localHeadshot = 19746 + 1740 + 1; // 总的爆头数
    const static int localKillHost = 19746 + 1725 + 1; // 房主击杀数/首轮北边击杀总和/M1击杀总和
    const static int localKillClient1 = 19746 + 1725 + 2; // 队友1击杀数/首轮南边击杀总和
    const static int localVehicleDamage = 24580 + 1231; // 车辆受损程度
    const static int localRashkovskyDamage = 24580 + 1230; // 拉什科夫斯基受伤程度
    const static int localHeadshotHost = 31621 + 1 + (0 * 292) + 85; // 房主爆头数
    const static int localHeadshotClient1 = 31621 + 1 + (1 * 292) + 85; // 队友1爆头数

    // 结算页面的数据
    const static int globalTime = 2685444 + 3498 + 95; // 时间
    const static int globalPrisonBreakTime = 2685444 + 3498 + 107; // 越狱的逃离时间
    const static int globalHeadshot = 2685444 + 3498 + 115; // 总的爆头数
    const static int globalKill = 2685444 + 3498 + 111; // 总的击杀数
    const static int globalVehicleDamage = 2685444 + 3498 + 99; // 车辆受损程度
    const static int globalRashkovskyDamage = 2685444 + 3498 + 103; // 拉什科夫斯基受伤程度

    const static int localInitTimestamp = 19746 + 985; // 开始计时时的时间戳
    const static int localInitPrisonBreakTimestamp = 19746 + 988; // 开始计时时的时间戳
    const static int localFlagInitTimer = 19746 + 985 + 1; // 是否开始计时，1开始，0暂停
    const static int localFlagInitPrisonBreakTimer = 19746 + 988 + 1; // 越狱逃离是否开始计时，1开始，0暂停

    // 任务HASH
    inline const static unsigned long long hashDoomsdayAct1 = 1175383697; // 末日1分红
    inline const static unsigned long long hashDoomsdayAct2 = 3883215059; // 末日2分红
    inline const static unsigned long long hashDoomsdayAct3P1 = 3118746651; // 末日3分红前一段
    inline const static unsigned long long hashDoomsdayAct3P2 = 4263845131; // 末日3分红后一段
    inline const static unsigned long long hashFleecaJob = 3222096535; // 全福分红
    inline const static unsigned long long hashPrisonBreakStation = 2569787839; // 越狱：警察局
    inline const static unsigned long long hashPrisonBreakWetWork = 137052480; // 越狱：行刺
    inline const static unsigned long long hashPrisonBreak = 979654579; // 越狱分红
    inline const static unsigned long long hashHumaneLabsRaid = 3197980642; // 人道分红
    inline const static unsigned long long hashSeriesAFunding = 164435858; // 首轮分红
    inline const static unsigned long long hashPacificStandardP1 = 4062993727; // 太平洋分红前一段
    inline const static unsigned long long hashPacificStandardP2 = 1309288309; // 太平洋分红后一段
    inline const static unsigned long long hashCasinoHeistAggressive1 = 14334224; // 赌场：气势汹汹
    inline const static unsigned long long hashCasinoHeistAggressive2 = 1142061386; // 赌场：气势汹汹
    inline const static unsigned long long hashCasinoHeistAggressive3 = 2648873158; // 赌场：气势汹汹
    inline const static unsigned long long hashCasinoHeistAggressive4 = 3081071674; // 赌场：气势汹汹
    inline const static unsigned long long hashCasinoHeistAggressive5 = 3554048209; // 赌场：气势汹汹
    inline const static unsigned long long hashCasinoHeistSilentSneaky1 = 2971240475; // 赌场：隐迹潜踪
    inline const static unsigned long long hashCasinoHeistSilentSneaky2 = 784080617; // 赌场：隐迹潜踪
    inline const static unsigned long long hashCasinoHeistSilentSneaky3 = 2305332202; // 赌场：隐迹潜踪
    inline const static unsigned long long hashCasinoHeistSilentSneaky4 = 2429084324; // 赌场：隐迹潜踪
    inline const static unsigned long long hashCasinoHeistTheBigCon1 = 3189252359; // 赌场：兵不厌诈
    inline const static unsigned long long hashCasinoHeistTheBigCon2 = 2453212730; // 赌场：兵不厌诈
    inline const static unsigned long long hashCasinoHeistTheBigCon3 = 3077120672; // 赌场：兵不厌诈
    inline const static unsigned long long hashCasinoHeistTheBigCon4 = 3528605896; // 赌场：兵不厌诈
    inline const static unsigned long long hashCasinoHeistEndStage = 2981742808; // 赌场。三个方式的最后阶段
    inline const static unsigned long long hashLostMcRip = 2389264995; // 安息吧！失落摩托帮
    inline const static unsigned long long hashBakerBadBeat = 2805392149; // 贝克女士：出奇制胜

    HWND getWindowHwnd();
    HANDLE getProcessHandle(DWORD* pid, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS);
    HMODULE getProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName);

    WINBOOL read(unsigned long long address, LPVOID buffer, SIZE_T size);
    unsigned long long findPattern(QString pattern);
    unsigned long long rip37(unsigned long long address);

    bool initGlobalPtr();
    void initMissionPtr();

    int getGlobalInt(int index);
    int getGlobalUInt(int index);
    long long getGlobalLongLong(int index);
    unsigned long long getGlobalULongLong(int index);
    float getGlobalFloat(int index);

    int getLocalInt(int index);
    unsigned int getLocalUInt(int index);
    float getLocalFloat(int index);
    long long getLocalLongLong(int index);
    unsigned long long getLocalULongLong(int index);
    bool getLocalBool(int index);

    // 恶意值
    int getBadSport();

private:
    QTimer* gtaProcessTimer = new QTimer(this);
signals:
    void onMissionPtrChanged();
};
