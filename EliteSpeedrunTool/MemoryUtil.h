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
    const static int localKill1 = 19746 + 1725 + 1; // 房主击杀数/首轮北边击杀总和/M1击杀总和
    const static int localKill2 = 19746 + 1725 + 2; // 队友1击杀数/首轮南边击杀总和
    const static int localKill3 = 19746 + 1725 + 3;
    const static int localKill4 = 19746 + 1725 + 4;
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
    // ACT1
    inline const static unsigned long long hashTheDataBreaches = 1175383697; // 末日1分红
    inline const static unsigned long long hashAct1DeadCourier = 2310376779; // 准备任务：亡命速递
    inline const static unsigned long long hashAct1SignalIntercepts = 2988089418; // 准备任务：拦截信号
    inline const static unsigned long long hashAct1ServerFarm = 83978007; // 准备任务：服务器群组
    // ACT2
    inline const static unsigned long long hashTheBogdanProblem = 3883215059; // 末日2分红
    inline const static unsigned long long hashAct2Avenger = 1673641696; // 准备任务：复仇者
    inline const static unsigned long long hashAct2RescueUlp = 1549726198; // 准备任务：营救 ULP
    inline const static unsigned long long hashAct2SalvageHardDrives = 1146411126; // 准备任务：抢救硬盘
    inline const static unsigned long long hashAct2SubmarineRecon = 1981951486; // 准备任务：潜水艇侦察
    // ACT3
    inline const static unsigned long long hashTheDoomsdayScenarioP1 = 3118746651; // 末日3分红前一段
    inline const static unsigned long long hashTheDoomsdayScenarioP2 = 4263845131; // 末日3分红后一段
    inline const static unsigned long long hashAct3RescueAgent14 = 1865386823; // 准备任务：营救 14 号探员
    inline const static unsigned long long hashAct3EscortUlp = 1374735669; // 准备任务：护送 ULP
    inline const static unsigned long long hashAct3Barrage = 2643765252; // 准备任务：巴拉杰
    inline const static unsigned long long hashAct3Khanjali = 1579954143; // 准备任务：可汗贾利
    inline const static unsigned long long hashAct3AirDefenses = 4184233611; // 准备任务：空中防御
    // 全福
    inline const static unsigned long long hashFleecaJobScopeOut = 1771792820; // 全福探查
    inline const static unsigned long long hashFleecaJobKuruma = 3458614835; // 全福骷髅马
    inline const static unsigned long long hashTheFleecaJob = 3222096535; // 全福分红
    // 越狱
    inline const static unsigned long long hashPrisonBreakPlane = 793579785; // 越狱：飞机
    inline const static unsigned long long hashPrisonBreakBus = 2652883801; // 越狱：巴士
    inline const static unsigned long long hashPrisonBreakStation = 2569787839; // 越狱：警察局
    inline const static unsigned long long hashPrisonBreakWetWork = 842031918; // 越狱：行刺
    inline const static unsigned long long hashThePrisonBreak = 979654579; // 越狱分红
    // 人道
    inline const static unsigned long long hashHumaneRaidKeyCodes1 = 466825080; // 突袭人道研究实验室：关键密码1
    inline const static unsigned long long hashHumaneRaidKeyCodes2 = 3348094115; // 突袭人道研究实验室：关键密码2
    inline const static unsigned long long hashHumaneRaidInsurgents = 2039847454; // 突袭人道研究实验室：叛乱分子
    inline const static unsigned long long hashHumaneRaidEmp = 2136235844; // 突袭人道研究实验室：电磁脉冲装置
    inline const static unsigned long long hashHumaneRaidValkyrie = 1324253820; // 突袭人道研究实验室：女武神
    inline const static unsigned long long hashHumaneRaidDeliverEmp = 496643418; // 突袭人道研究实验室：运送电磁脉冲装置
    inline const static unsigned long long hashTheHumaneLabsRaid = 3197980642; // 突袭人道研究实验室 分红
    // 首轮
    inline const static unsigned long long hashSeriesACoke = 3704629663; // 首轮募资：可卡因
    inline const static unsigned long long hashSeriesATrashTruck = 1571421055; // 首轮募资：垃圾车
    inline const static unsigned long long hashSeriesABikers = 3664952125; // 首轮募资：摩托车手
    inline const static unsigned long long hashSeriesAWeed = 4280918227; // 首轮募资：大麻
    inline const static unsigned long long hashSeriesAStealMeth = 1585746186; // 首轮募资：窃取冰毒
    inline const static unsigned long long hashSeriesAFunding = 164435858; // 募资 分红
    // 太平洋
    inline const static unsigned long long hashThePacificStandardP1 = 4062993727; // 太平洋分红前一段
    inline const static unsigned long long hashThePacificStandardP2 = 1309288309; // 太平洋分红后一段
    inline const static unsigned long long hashPacificStandardVans = 3141270655; // 太平洋：厢型车
    inline const static unsigned long long hashPacificStandardSignal = 124038394; // 太平洋：信号
    inline const static unsigned long long hashPacificStandardHack = 2698675922; // 太平洋：破解
    inline const static unsigned long long hashPacificStandardConvoy = 1756125549; // 太平洋：车队
    inline const static unsigned long long hashPacificStandardBikes = 911181645; // 太平洋：摩托车
    // 赌场
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
    inline const static unsigned long long hashCasinoBadBeat = 2805392149; // 贝克女士：出奇制胜

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
