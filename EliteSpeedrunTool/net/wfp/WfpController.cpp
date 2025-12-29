#define INITGUID
#include "WfpController.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <Shlwapi.h>
#include <Windows.h>
#include <fwpmu.h>
#include <fwptypes.h>
#include <ws2tcpip.h>

Q_GLOBAL_STATIC(QWfpController, wfpControllerInstance)

// 默认的 Provider 和 Sublayer GUIDs
static const GUID kDefaultProviderGuid = {
    0x4310d91e, 0xf336, 0x43f3,
    { 0xa2, 0x10, 0x88, 0x68, 0x48, 0xef, 0xc1, 0x4a }
};

static const GUID kDefaultSublayerGuid = {
    0x9946b99e, 0x15fb, 0x4371,
    { 0x9c, 0xa3, 0x84, 0xbe, 0x8b, 0x9b, 0x42, 0xad }
};

#define FWPM_FILTER_FLAG_INDEXED (0x00000040)

// c38d57d1-05a7-4c33-904f-7fbceee60e82
DEFINE_GUID(
    FWPM_LAYER_ALE_AUTH_CONNECT_V4,
    0xc38d57d1,
    0x05a7,
    0x4c33,
    0x90, 0x4f, 0x7f, 0xbc, 0xee, 0xe6, 0x0e, 0x82);

// e1cd9fe7-f4b5-4273-96c0-592e487b8650
DEFINE_GUID(
    FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V4,
    0xe1cd9fe7,
    0xf4b5,
    0x4273,
    0x96, 0xc0, 0x59, 0x2e, 0x48, 0x7b, 0x86, 0x50);

// d78e1e87-8644-4ea5-9437-d809ecefc971
DEFINE_GUID(
    FWPM_CONDITION_ALE_APP_ID,
    0xd78e1e87,
    0x8644,
    0x4ea5,
    0x94, 0x37, 0xd8, 0x09, 0xec, 0xef, 0xc9, 0x71);

// b235ae9a-1d64-49b8-a44c-5ff3d9095045
DEFINE_GUID(
    FWPM_CONDITION_IP_REMOTE_ADDRESS,
    0xb235ae9a,
    0x1d64,
    0x49b8,
    0xa4, 0x4c, 0x5f, 0xf3, 0xd9, 0x09, 0x50, 0x45);

// c35a604d-d22b-4e1a-91b4-68f674ee674b
DEFINE_GUID(
    FWPM_CONDITION_IP_REMOTE_PORT,
    0xc35a604d,
    0xd22b,
    0x4e1a,
    0x91, 0xb4, 0x68, 0xf6, 0x74, 0xee, 0x67, 0x4b);

// 3971ef2b-623e-4f9a-8cb1-6e79b806b9a7
DEFINE_GUID(
    FWPM_CONDITION_IP_PROTOCOL,
    0x3971ef2b,
    0x623e,
    0x4f9a,
    0x8c, 0xb1, 0x6e, 0x79, 0xb8, 0x06, 0xb9, 0xa7);

// 0c1ba1af-5765-453f-af22-a8f791ac775b
DEFINE_GUID(
    FWPM_CONDITION_IP_LOCAL_PORT,
    0x0c1ba1af,
    0x5765,
    0x453f,
    0xaf, 0x22, 0xa8, 0xf7, 0x91, 0xac, 0x77, 0x5b);

class QWfpControllerPrivate {
public:
    QWfpControllerPrivate(QWfpController* qq);
    ~QWfpControllerPrivate();

    // 初始化相关
    bool openWfpEngine();
    bool initializeProvider();
    bool initializeWinsock();

    // 规则操作
    qint64 createFilterRule(const QWfpController::RuleCondition& condition,
        const QString& ruleName, QString& errorMsg);
    bool cleanupAllFilters(QString& errorMsg);

    // 工具函数
    static FWP_BYTE_BLOB* createAppIdFromPath(const QString& path);
    static void freeAppIdBlob(FWP_BYTE_BLOB* blob);
    static QString getLastErrorMessage(DWORD errorCode);
    static QString guidToString(const GUID& guid);
    static bool isEqualGuid(const GUID& g1, const GUID& g2);

    // 事务管理
    bool beginTransaction();
    bool commitTransaction();
    void abortTransaction();

    // 成员变量
    QWfpController* q_ptr;
    HANDLE engineHandle = nullptr;
    bool isEngineOpen = false;
    bool transactionBegun = false;
    QVector<QWfpController::RuleInfo> createdRules;
    QElapsedTimer operationTimer;

private:
    Q_DECLARE_PUBLIC(QWfpController)
};

// ==================== QWfpController 实现 ====================

QWfpController::QWfpController(QObject* parent)
    : QObject(parent)
    , d_ptr(new QWfpControllerPrivate(this))
{
}

QWfpController::~QWfpController()
{
    shutdown();
}

QWfpController* QWfpController::instance()
{
    return wfpControllerInstance;
}

QWfpController::OperationResult QWfpController::initialize()
{
    Q_D(QWfpController);

    OperationResult result;
    d->operationTimer.start();

    // 初始化 Winsock
    if (!d->initializeWinsock()) {
        result.errorMessage = "初始化 Winsock 失败";
        result.errorCode = WSAGetLastError();
        emit errorOccurred(result.errorMessage, result.errorCode);
        return result;
    }

    // 打开 WFP 引擎
    if (!d->openWfpEngine()) {
        result.errorMessage = "打开 WFP 引擎失败";
        result.errorCode = GetLastError();
        WSACleanup();
        emit errorOccurred(result.errorMessage, result.errorCode);
        return result;
    }

    // 初始化 Provider
    if (!d->initializeProvider()) {
        result.errorMessage = "初始化 WFP Provider 失败";
        result.errorCode = GetLastError();
        d->engineHandle = nullptr;
        WSACleanup();
        emit errorOccurred(result.errorMessage, result.errorCode);
        return result;
    }

    result.success = true;
    result.elapsedMs = d->operationTimer.elapsed();

    emit initialized(true);

    return result;
}

QWfpController::OperationResult QWfpController::createRule(
    const RuleCondition& condition, const QString& ruleName)
{
    Q_D(QWfpController);

    OperationResult result;
    d->operationTimer.start();

    if (!isInitialized()) {
        result.errorMessage = "WFP engine uninit";
        emit errorOccurred(result.errorMessage, 0);
        return result;
    }

    QString errorMsg;
    qint64 filterId = d->createFilterRule(condition, ruleName, errorMsg);

    if (filterId > 0) {
        result.success = true;
        result.ruleId = filterId;

        // 保存规则信息
        RuleInfo info;
        info.ruleId = QUuid::createUuid();
        info.name = ruleName.isEmpty()
            ? QString("%1 Rule %2").arg(qApp->applicationName(), QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"))
            : ruleName;
        info.description = condition.description();
        info.condition = condition;
        info.creationTime = QDateTime::currentMSecsSinceEpoch();

        d->createdRules.append(info);

        emit ruleCreated(info);
    } else {
        result.errorMessage = errorMsg;
        result.errorCode = GetLastError();
        emit errorOccurred(errorMsg, result.errorCode);
    }

    result.elapsedMs = d->operationTimer.elapsed();
    return result;
}

QList<QWfpController::OperationResult> QWfpController::createGlobalBlockRule(const QString& ruleName)
{
    RuleCondition inCondition;
    inCondition.direction = FWP_DIRECTION_INBOUND;
    RuleCondition outCondition;
    outCondition.direction = FWP_DIRECTION_OUTBOUND;
    return { createRule(inCondition, ruleName.isEmpty() ? "Global Block All InBound" : ruleName),
        createRule(outCondition, ruleName.isEmpty() ? "Global Block All OutBound" : ruleName) };
}

QWfpController::OperationResult QWfpController::cleanupAllRules()
{
    Q_D(QWfpController);

    OperationResult result;
    d->operationTimer.start();

    if (!isInitialized()) {
        result.errorMessage = "WFP engine not initialized";
        emit errorOccurred(result.errorMessage, 0);
        return result;
    }

    QString errorMsg;
    if (d->cleanupAllFilters(errorMsg)) {
        result.success = true;

        // 发送删除信号
        for (const auto& rule : d->createdRules) {
            emit ruleDeleted(rule.ruleId.toUInt128());
        }

        d->createdRules.clear();

        emit cleanupCompleted(d->createdRules.size());
    } else {
        result.errorMessage = errorMsg;
        result.errorCode = GetLastError();
        emit errorOccurred(errorMsg, result.errorCode);
    }

    result.elapsedMs = d->operationTimer.elapsed();
    return result;
}

QVector<QWfpController::RuleInfo> QWfpController::getAllRules() const
{
    Q_D(const QWfpController);
    return d->createdRules;
}

bool QWfpController::isInitialized() const
{
    Q_D(const QWfpController);
    return d->engineHandle != nullptr && d->isEngineOpen;
}

void QWfpController::shutdown()
{
    Q_D(QWfpController);

    // 删除子层和提供者
    FwpmSubLayerDeleteByKey0(d->engineHandle, &kDefaultSublayerGuid);
    FwpmProviderDeleteByKey0(d->engineHandle, &kDefaultProviderGuid);
    if (d->engineHandle) {
        if (d->transactionBegun) {
            d->abortTransaction();
        }

        FwpmEngineClose0(d->engineHandle);
        d->engineHandle = nullptr;
        d->isEngineOpen = false;

        WSACleanup();
    }
}

QString QWfpController::RuleCondition::description() const
{
    QStringList parts;

    if (hasProcess()) {
        QString fileName = QFileInfo(processPath).fileName();
        parts.append(QString("Process: %1").arg(fileName));
    }

    if (hasRemoteIp()) {
        parts.append(QString("IP: %1").arg(remoteIp.toString()));
    }

    if (hasRemotePort()) {
        parts.append(QString("Remote port: %1").arg(remotePort));
    }

    if (hasLocalPort()) {
        parts.append(QString("Local port: %1").arg(localPort));
    }

    if (hasDirection() && direction == FWP_DIRECTION_INBOUND) {
        parts.append("InBound");
    } else {
        parts.append("OutBound");
    }

    if (hasProtocol()) {
        QString protoStr;
        switch (protocol) {
        case TCP:
            protoStr = "TCP";
            break;
        case UDP:
            protoStr = "UDP";
            break;
        case ICMP:
            protoStr = "ICMP";
            break;
        default:
            protoStr = QString("Protocol %1").arg(static_cast<int>(protocol));
        }
        parts.append(protoStr);
    }

    if (parts.isEmpty()) {
        return "Block all connections";
    }

    return parts.join(" + ");
}

// ==================== QWfpControllerPrivate 实现 ====================

QWfpControllerPrivate::QWfpControllerPrivate(QWfpController* qq)
    : q_ptr(qq)
{
}

QWfpControllerPrivate::~QWfpControllerPrivate()
{
    if (engineHandle) {
        FwpmEngineClose0(engineHandle);
    }
}

bool QWfpControllerPrivate::openWfpEngine()
{
    FWPM_SESSION0 session = {};
    std::wstring name = L"Elite Speedrun Tool WFP Controller";
    session.displayData.name = &name[0];
    std::wstring description = L"Elite Speedrun Tool WFP Firewall Control Tool";
    session.displayData.description = &description[0];
    session.flags = 0;
    session.txnWaitTimeoutInMSec = 5000; // 5秒超时

    DWORD status = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL,
        &session, &engineHandle);

    if (status != ERROR_SUCCESS) {
        qWarning() << "Failed to open WFP engine:"
                   << getLastErrorMessage(status);
        return false;
    }

    isEngineOpen = true;
    return true;
}

bool QWfpControllerPrivate::initializeProvider()
{
    // 检查 Provider 是否已存在
    FWPM_PROVIDER0* existingProvider = NULL;
    DWORD status = FwpmProviderGetByKey0(engineHandle, &kDefaultProviderGuid,
        &existingProvider);

    if (status == ERROR_SUCCESS && existingProvider != NULL) {
        FwpmFreeMemory0((void**)&existingProvider);
        return true;
    }

    // Create Provider
    FWPM_PROVIDER0 provider = {};
    std::wstring providerName = L"Elite Speedrun Tool Provider";
    provider.displayData.name = &providerName[0];
    std::wstring providerDescription = L"Elite Speedrun Tool Firewall Provider";
    provider.displayData.description = &providerDescription[0];
    provider.providerKey = kDefaultProviderGuid;
    provider.flags = 0;

    status = FwpmProviderAdd0(engineHandle, &provider, NULL);
    if (status != ERROR_SUCCESS && status != FWP_E_ALREADY_EXISTS) {
        qWarning() << "Failed to add provider:" << getLastErrorMessage(status);
        return false;
    }

    // Create Sublayer
    FWPM_SUBLAYER0 sublayer = {};
    std::wstring sublayerName = L"Elite Speedrun Tool Sublayer";
    sublayer.displayData.name = &sublayerName[0];
    std::wstring sublayerDescription = L"Elite Speedrun Tool Firewall Sublayer";
    sublayer.displayData.description = &sublayerDescription[0];
    sublayer.providerKey = const_cast<GUID*>(&kDefaultProviderGuid);
    sublayer.subLayerKey = kDefaultSublayerGuid;
    sublayer.weight = 0xFFFF; // 最高权重

    status = FwpmSubLayerAdd0(engineHandle, &sublayer, NULL);
    if (status != ERROR_SUCCESS && status != FWP_E_ALREADY_EXISTS) {
        qWarning() << "Failed to add sublayer:" << getLastErrorMessage(status);
        return false;
    }

    return true;
}

bool QWfpControllerPrivate::initializeWinsock()
{
    WSADATA wsaData;
    return (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
}

qint64 QWfpControllerPrivate::createFilterRule(
    const QWfpController::RuleCondition& condition,
    const QString& ruleName, QString& errorMsg)
{
    if (!beginTransaction()) {
        errorMsg = "Begin transaction failed";
        return 0;
    }

    bool success = false;
    qint64 filterId = 0;
    FWP_BYTE_BLOB* appBlob = nullptr;

    try {
        // 准备过滤器
        FWPM_FILTER0 filter = {};
        UuidCreate(&filter.filterKey);

        auto isInBound = condition.hasDirection() && condition.direction == FWP_DIRECTION_INBOUND;
        if (isInBound) {
            filter.layerKey = FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V4;
        } else {
            filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
        }

        // 设置显示信息
        QString displayName = ruleName.isEmpty() ? QString("QtWFP Rule %1").arg(QDateTime::currentDateTime().toString("hhmmss")) : ruleName;

        std::wstring wName = displayName.toStdWString();
        std::wstring wDesc = condition.description().toStdWString();

        filter.displayData.name = const_cast<wchar_t*>(wName.c_str());
        filter.displayData.description = const_cast<wchar_t*>(wDesc.c_str());

        filter.subLayerKey = kDefaultSublayerGuid;
        filter.weight.type = FWP_EMPTY;
        filter.flags = FWPM_FILTER_FLAG_INDEXED;
        filter.action.type = FWP_ACTION_BLOCK;
        filter.providerKey = (GUID*)&kDefaultProviderGuid;

        // 构建条件数组
        QVector<FWPM_FILTER_CONDITION0> conditions;

        // 1. 进程条件
        if (condition.hasProcess()) {
            appBlob = createAppIdFromPath(condition.processPath);
            if (!appBlob) {
                throw std::runtime_error("Cannot create app ID");
            }

            FWPM_FILTER_CONDITION0 cond = {};
            cond.fieldKey = FWPM_CONDITION_ALE_APP_ID;
            cond.matchType = FWP_MATCH_EQUAL;
            cond.conditionValue.type = FWP_BYTE_BLOB_TYPE;
            cond.conditionValue.byteBlob = appBlob;
            conditions.append(cond);
        }

        // 2. IP地址条件
        if (condition.hasRemoteIp()) {
            quint32 ipValue = condition.remoteIp.toIPv4Address();

            FWPM_FILTER_CONDITION0 cond = {};
            cond.fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
            cond.matchType = FWP_MATCH_EQUAL;
            cond.conditionValue.type = FWP_UINT32;
            cond.conditionValue.uint32 = htonl(ipValue); // 转为网络字节序
            conditions.append(cond);
        }

        // 3. 远程端口条件
        if (condition.hasRemotePort()) {
            FWPM_FILTER_CONDITION0 cond = {};
            cond.fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
            cond.matchType = FWP_MATCH_EQUAL;
            cond.conditionValue.type = FWP_UINT16;
            cond.conditionValue.uint16 = condition.remotePort;
            conditions.append(cond);
        }

        // 4. 本地端口条件
        if (condition.hasLocalPort()) {
            FWPM_FILTER_CONDITION0 cond = {};
            cond.fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
            cond.matchType = FWP_MATCH_EQUAL;
            cond.conditionValue.type = FWP_UINT16;
            cond.conditionValue.uint16 = condition.localPort;
            conditions.append(cond);
        }

        // 5. 协议条件
        if (condition.hasProtocol()) {
            FWPM_FILTER_CONDITION0 cond = {};
            cond.fieldKey = FWPM_CONDITION_IP_PROTOCOL;
            cond.matchType = FWP_MATCH_EQUAL;
            cond.conditionValue.type = FWP_UINT8;
            cond.conditionValue.uint8 = static_cast<UINT8>(condition.protocol);
            conditions.append(cond);
        }

        // 设置条件数组
        filter.numFilterConditions = static_cast<UINT32>(conditions.size());
        if (!conditions.isEmpty()) {
            filter.filterCondition = conditions.data();
        }

        // 添加过滤器
        UINT64 addedFilterId = 0;
        DWORD status = FwpmFilterAdd0(engineHandle, &filter, NULL, &addedFilterId);

        if (status != ERROR_SUCCESS) {
            errorMsg = QString("Add filter failed: %1").arg(getLastErrorMessage(status));
            throw std::runtime_error(errorMsg.toStdString());
        }

        filterId = static_cast<qint64>(addedFilterId);
        success = true;

        qDebug() << "Filter create succeed, ID:" << filterId << "condition:" << condition.description();

    } catch (const std::exception& e) {
        errorMsg = QString::fromLocal8Bit(e.what());
    }

    // 清理资源
    if (appBlob) {
        freeAppIdBlob(appBlob);
    }

    // 提交或回滚事务
    if (success) {
        if (!commitTransaction()) {
            errorMsg = "Commit transaction failed";
            filterId = 0;
        }
    } else {
        abortTransaction();
    }

    return filterId;
}

bool QWfpControllerPrivate::cleanupAllFilters(QString& errorMsg)
{
    if (!beginTransaction()) {
        errorMsg = "Begin transaction failed";
        return false;
    }

    int deletedCount = 0;
    DWORD status = ERROR_SUCCESS;
    HANDLE enumHandle = NULL;

    try {
        // 创建枚举句柄
        status = FwpmFilterCreateEnumHandle0(engineHandle, NULL, &enumHandle);
        if (status != ERROR_SUCCESS) {
            throw std::runtime_error(
                QString("Create enum handle error: %1").arg(getLastErrorMessage(status)).toStdString());
        }

        FWPM_FILTER0** filters = NULL;
        UINT32 numFilters = 0;

        // 分页枚举所有过滤器
        do {
            status = FwpmFilterEnum0(engineHandle, enumHandle, 512, &filters, &numFilters);
            if (status != ERROR_SUCCESS) {
                break;
            }

            if (numFilters == 0) {
                FwpmFreeMemory0((void**)&filters);
                break;
            }

            // 检查并删除匹配的过滤器
            for (UINT32 i = 0; i < numFilters; i++) {
                if (!filters[i])
                    continue;

                bool shouldDelete = false;

                // 检查 Provider 匹配
                if (filters[i]->providerKey) {
                    shouldDelete = isEqualGuid(*filters[i]->providerKey, kDefaultProviderGuid);
                }

                // 检查 Sublayer 匹配
                if (!shouldDelete) {
                    shouldDelete = isEqualGuid(filters[i]->subLayerKey, kDefaultSublayerGuid);
                }

                // 检查名称匹配（包含 QtWFP 字样）
                if (!shouldDelete && filters[i]->displayData.name) {
                    QString name = QString::fromWCharArray(filters[i]->displayData.name);
                    shouldDelete = name.contains(qApp->applicationName(), Qt::CaseInsensitive);
                }

                if (shouldDelete) {
                    status = FwpmFilterDeleteByKey0(engineHandle, &filters[i]->filterKey);
                    if (status == ERROR_SUCCESS) {
                        deletedCount++;
                    }
                }
            }

            FwpmFreeMemory0((void**)&filters);
            filters = NULL;
            numFilters = 0;

        } while (status == ERROR_SUCCESS);
    } catch (const std::exception& e) {
        errorMsg = QString::fromLocal8Bit(e.what());
        if (enumHandle) {
            FwpmFilterDestroyEnumHandle0(engineHandle, enumHandle);
        }
        abortTransaction();
        return false;
    }

    if (enumHandle) {
        FwpmFilterDestroyEnumHandle0(engineHandle, enumHandle);
    }

    // 提交事务
    if (!commitTransaction()) {
        errorMsg = "Transaction commit failed";
        abortTransaction();
        return false;
    }

    qDebug() << "Clear complete, " << deletedCount << "filters deleted";
    return true;
}

FWP_BYTE_BLOB* QWfpControllerPrivate::createAppIdFromPath(const QString& path)
{
    if (path.isEmpty()) {
        qWarning() << "path is empty";
        return nullptr;
    }

    // 确保路径使用原生分隔符（反斜杠）
    QString nativePath = QDir::toNativeSeparators(path);
    std::wstring wPath = nativePath.toStdWString();

    FWP_BYTE_BLOB* appBlob = nullptr;

    // 调用 FwpmGetAppIdFromFileName0 获取应用程序标识符
    // 函数签名：DWORD FwpmGetAppIdFromFileName0(PCWSTR fileName, FWP_BYTE_BLOB** appId)[reference:0]
    DWORD status = FwpmGetAppIdFromFileName0(wPath.c_str(), &appBlob);

    if (status != ERROR_SUCCESS) {
        qWarning() << "FwpmGetAppIdFromFileName0 failed, path:" << nativePath
                   << "err:" << getLastErrorMessage(status);
        return nullptr;
    }

    if (appBlob == nullptr) {
        qWarning() << "FwpmGetAppIdFromFileName0 return null, path:" << nativePath;
        return nullptr;
    }

    qDebug() << "Create app path ID success, path:" << nativePath << "data size:" << appBlob->size;

    return appBlob;
}

void QWfpControllerPrivate::freeAppIdBlob(FWP_BYTE_BLOB* blob)
{
    if (blob) {
        FwpmFreeMemory0((void**)&blob);
    }
}

QString QWfpControllerPrivate::getLastErrorMessage(DWORD errorCode)
{
    if (errorCode == 0) {
        errorCode = GetLastError();
    }

    LPWSTR msgBuf = nullptr;
    DWORD msgLen = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&msgBuf, 0, NULL);

    QString result;
    if (msgLen > 0) {
        result = QString::fromWCharArray(msgBuf, msgLen).trimmed();
        LocalFree(msgBuf);
    } else {
        result = QString("unknown err: (code: 0x%1)").arg(errorCode, 8, 16, QChar('0'));
    }

    return result;
}

QString QWfpControllerPrivate::guidToString(const GUID& guid)
{
    return QString("%1-%2-%3-%4-%5")
        .arg(guid.Data1, 8, 16, QChar('0'))
        .arg(guid.Data2, 4, 16, QChar('0'))
        .arg(guid.Data3, 4, 16, QChar('0'))
        .arg(QString::fromLatin1(QByteArray(reinterpret_cast<const char*>(guid.Data4), 2).toHex()),
            QString::fromLatin1(QByteArray(reinterpret_cast<const char*>(guid.Data4 + 2), 6).toHex()));
}

bool QWfpControllerPrivate::isEqualGuid(const GUID& g1, const GUID& g2)
{
    return IsEqualGUID(g1, g2) != FALSE;
}

bool QWfpControllerPrivate::beginTransaction()
{
    if (transactionBegun) {
        return true; // 事务已开始
    }

    DWORD status = FwpmTransactionBegin0(engineHandle, 0);
    if (status != ERROR_SUCCESS && status != FWP_E_TXN_IN_PROGRESS) {
        qWarning() << "Begin transaction failed:" << getLastErrorMessage(status);
        return false;
    }

    transactionBegun = (status == ERROR_SUCCESS);
    return true;
}

bool QWfpControllerPrivate::commitTransaction()
{
    if (!transactionBegun) {
        return true; // 没有需要提交的事务
    }

    DWORD status = FwpmTransactionCommit0(engineHandle);
    transactionBegun = false;

    if (status != ERROR_SUCCESS) {
        qWarning() << "Commit transaction failed:" << getLastErrorMessage(status);
        return false;
    }

    return true;
}

void QWfpControllerPrivate::abortTransaction()
{
    if (transactionBegun) {
        FwpmTransactionAbort0(engineHandle);
        transactionBegun = false;
    }
}
