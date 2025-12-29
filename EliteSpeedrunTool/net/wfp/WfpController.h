#pragma once

#include <QElapsedTimer>
#include <QHostAddress>
#include <QObject>
#include <QString>
#include <QUuid>
#include <QVector>
#include <memory>
#include <winsock2.h>

#define wfpController (QWfpController::instance())

class QWfpControllerPrivate;

/**
 * @brief Qt 封装的 Windows Filtering Platform (WFP) 防火墙控制器
 *
 * 这个类提供了基于应用程序进程、IP地址、端口和协议的网络连接控制功能。
 * 支持创建精细的阻断规则，以及清理所有创建的规则。
 */
class QWfpController : public QObject {
    Q_OBJECT

public:
    /// 协议类型枚举
    enum Protocol {
        AnyProtocol = 0, ///< 任意协议
        TCP = 6, ///< TCP 协议 (IPPROTO_TCP)
        UDP = 17, ///< UDP 协议 (IPPROTO_UDP)
        ICMP = 1 ///< ICMP 协议
    };
    Q_ENUM(Protocol)

    /// 规则条件结构体
    struct RuleCondition {
        QString processPath; ///< 进程完整路径 (必须完整路径)
        QHostAddress remoteIp; ///< 远程 IP 地址
        quint16 remotePort = 0; ///< 远程端口
        quint16 localPort = 0; ///< 本地端口
        Protocol protocol = AnyProtocol; ///< 协议类型
        qint32 direction = -1; ///< 协议类型

        bool hasProcess() const { return !processPath.isEmpty(); }
        bool hasRemoteIp() const { return !remoteIp.isNull(); }
        bool hasRemotePort() const { return remotePort > 0; }
        bool hasLocalPort() const { return localPort > 0; }
        bool hasProtocol() const { return protocol != AnyProtocol; }
        bool hasDirection() const { return direction != -1; }

        /// 是否为空条件（将创建全局规则）
        bool isEmpty() const
        {
            return !hasProcess() && !hasRemoteIp() && !hasRemotePort() && !hasLocalPort() && !hasProtocol() && !hasDirection();
        }

        /// 生成规则的描述文本
        QString description() const;
    };

    /// 防火墙规则信息
    struct RuleInfo {
        QUuid ruleId; ///< 规则唯一ID
        QString name; ///< 规则名称
        QString description; ///< 规则描述
        RuleCondition condition; ///< 规则条件
        qint64 creationTime = 0; ///< 创建时间戳
    };

    /// 操作结果
    struct OperationResult {
        bool success = false; ///< 是否成功
        QString errorMessage; ///< 错误信息
        quint32 errorCode = 0; ///< 错误代码
        qint64 ruleId = 0; ///< 创建的规则ID
        qint64 elapsedMs = 0; ///< 操作耗时(毫秒)

        operator bool() const { return success; }
    };

public:
    explicit QWfpController(QObject* parent = nullptr);
    ~QWfpController();

    static QWfpController* instance();

    /**
     * @brief 初始化 WFP 引擎
     * @return 操作结果
     * @note 必须在使用其他功能前调用
     */
    OperationResult initialize();

    /**
     * @brief 创建防火墙规则
     * @param condition 规则条件
     * @param ruleName 规则名称（可选）
     * @return 操作结果
     */
    OperationResult createRule(const RuleCondition& condition, const QString& ruleName = QString());

    /**
     * @brief 创建全局阻断规则（阻断所有出站连接）
     * @param ruleName 规则名称（可选）
     * @return 操作结果
     */
    QList<QWfpController::OperationResult> createGlobalBlockRule(const QString& ruleName = QString());

    /**
     * @brief 清理所有本工具创建的规则
     * @return 操作结果
     */
    OperationResult cleanupAllRules();

    /**
     * @brief 获取所有创建的规则
     * @return 规则信息列表
     */
    QVector<RuleInfo> getAllRules() const;

    /**
     * @brief 检查是否已初始化
     * @return 已初始化返回true
     */
    bool isInitialized() const;

    /**
     * @brief 关闭 WFP 引擎
     */
    void shutdown();

signals:
    /**
     * @brief 规则创建信号
     * @param ruleInfo 创建的规则信息
     */
    void ruleCreated(const RuleInfo& ruleInfo);

    /**
     * @brief 规则删除信号
     * @param ruleId 被删除的规则ID
     */
    void ruleDeleted(quint128 ruleId);

    /**
     * @brief 操作错误信号
     * @param errorMessage 错误信息
     * @param errorCode 错误代码
     */
    void errorOccurred(const QString& errorMessage, quint32 errorCode);

    /**
     * @brief 初始化完成信号
     * @param success 是否成功
     */
    void initialized(bool success);

    /**
     * @brief 清理完成信号
     * @param deletedCount 删除的规则数量
     */
    void cleanupCompleted(int deletedCount);

private:
    std::unique_ptr<QWfpControllerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QWfpController)
    Q_DISABLE_COPY(QWfpController)
};
