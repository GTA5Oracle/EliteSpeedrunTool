#include "NetworkAdapterUtil.h"
#include <QDebug>
#include <Wbemidl.h>
#include <cfgmgr32.h>
#include <devguid.h>
#include <setupapi.h>

Q_GLOBAL_STATIC(NetworkAdapterUtil, networkAdapterUtilInstance)

NetworkAdapterUtil::NetworkAdapterUtil(QObject* parent)
    : QObject { parent }
{
    initNetworkAdapterGuid();
}

NetworkAdapterUtil* NetworkAdapterUtil::instance()
{
    return networkAdapterUtilInstance;
}

QList<NetworkAdapter> NetworkAdapterUtil::networkAdapters()
{
    QList<NetworkAdapter> adapters;

    // 获取网络适配器的设备信息集合
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, nullptr, nullptr, DIGCF_PRESENT | DIGCF_PROFILE);
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        qCritical() << "Failed to get device information set.";
        return adapters;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD deviceIndex = 0;

    // 遍历网络适配器
    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
        deviceIndex++;

        // 获取设备名称
        WCHAR deviceName[256];
        if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, nullptr,
                (PBYTE)deviceName, sizeof(deviceName), nullptr)) {
            qInfo() << "Device Name: " << QString::fromWCharArray(deviceName);
        } else {
            qCritical() << "Device Name: Unknown";
        }

        // 获取设备的实例 ID
        WCHAR deviceInstanceId[256];
        if (SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData, deviceInstanceId,
                sizeof(deviceInstanceId) / sizeof(WCHAR), nullptr)) {
            qInfo() << "Device Instance ID: " << QString::fromWCharArray(deviceInstanceId);
        } else {
            qCritical() << "Failed to get device instance ID for device index " << deviceIndex - 1;
        }

        adapters << NetworkAdapter(QString::fromWCharArray(deviceName), QString::fromWCharArray(deviceInstanceId));
    }

    // 清理设备信息集合
    SetupDiDestroyDeviceInfoList(deviceInfoSet);

    return adapters;
}

QList<QString> NetworkAdapterUtil::adapterNames(QList<QString> deviceIds)
{
    QList<QString> result;
    // 获取设备信息集合句柄
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(
        &networkAdapterGuid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_PROFILE);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        qCritical() << "Failed to get device information set.";
        return result;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD deviceIndex = 0;

    for (const auto& id : deviceIds) {
        if (!SetupDiOpenDeviceInfoA(deviceInfoSet, id.toStdString().c_str(), nullptr, 0, &deviceInfoData)) {
            qCritical() << "adapterName: Failed to open device with instance ID: " << id;
            SetupDiDestroyDeviceInfoList(deviceInfoSet);
            continue;
        }
        // 获取设备名称
        WCHAR deviceName[256];
        if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, nullptr,
                (PBYTE)deviceName, sizeof(deviceName), nullptr)) {
            result << QString::fromWCharArray(deviceName);
        } else {
            qCritical() << "Device Name: Unknown";
        }
    }
    qInfo() << "Device Names: " << result;

    return result;
}

bool NetworkAdapterUtil::setNetworkAdaptersEnabled(QList<QString> deviceIds, bool enabled)
{
    // 获取设备信息集合句柄
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(
        &networkAdapterGuid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_PROFILE);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        qCritical() << "Failed to get device information set.";
        return false;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD deviceIndex = 0;
    bool success = true;

    for (const auto& id : deviceIds) {
        if (!SetupDiOpenDeviceInfoA(deviceInfoSet, id.toStdString().c_str(), nullptr, 0, &deviceInfoData)) {
            qCritical() << "Failed to open device with instance ID: " << id;
            SetupDiDestroyDeviceInfoList(deviceInfoSet);
            success = false;
            break;
        }

        // 准备禁用设备的属性
        SP_PROPCHANGE_PARAMS propChangeParams;
        propChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
        propChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
        propChangeParams.StateChange = enabled ? DICS_ENABLE : DICS_DISABLE;
        propChangeParams.Scope = DICS_FLAG_GLOBAL;
        propChangeParams.HwProfile = 0;

        // 设置禁用参数
        if (!SetupDiSetClassInstallParams(deviceInfoSet, &deviceInfoData,
                (SP_CLASSINSTALL_HEADER*)&propChangeParams, sizeof(propChangeParams))) {
            qCritical() << "Failed to set class install parameters.";
            SetupDiDestroyDeviceInfoList(deviceInfoSet);
            success = false;
            break;
        }

        // 调用类安装程序执行禁用操作
        if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, deviceInfoSet, &deviceInfoData)) {
            qCritical() << "Failed to disable device with instance ID: " << id;
            SetupDiDestroyDeviceInfoList(deviceInfoSet);
            success = false;
            break;
        }
    }

    // 清理设备信息集合
    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return success;
}

void NetworkAdapterUtil::initNetworkAdapterGuid()
{
    // 获取所有网络适配器的设备信息
    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &GUID_DEVCLASS_NET, nullptr, nullptr, DIGCF_PRESENT | DIGCF_PROFILE);

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        qCritical() << "Failed to get device information.";
        return;
    }

    // 枚举设备信息
    SP_DEVINFO_DATA devInfoData;
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD i = 0;

    while (SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData)) {
        i++;

        // 获取设备实例 ID
        WCHAR instanceId[MAX_DEVICE_ID_LEN];
        if (SetupDiGetDeviceInstanceId(hDevInfo, &devInfoData, instanceId, MAX_DEVICE_ID_LEN, nullptr)) {
            qInfo() << "Network Adapter Device Instance ID: " << QString::fromWCharArray(instanceId);
        }

        // 获取设备的友好名称
        WCHAR friendlyName[256];
        if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr,
                (PBYTE)friendlyName, sizeof(friendlyName), nullptr)) {
            qInfo() << "Network Adapter Friendly Name: " << QString::fromWCharArray(friendlyName);
        }

        // 获取网络适配器的 GUID
        WCHAR classGuid[39] = { 0 };
        if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_CLASSGUID, nullptr,
                (PBYTE)&classGuid, sizeof(classGuid), nullptr)) {
            HRESULT hr = CLSIDFromString(classGuid, (LPCLSID)&networkAdapterGuid);
            if (FAILED(hr)) {
                qCritical() << "CLSIDFromString failed.";
            }
            qInfo() << "Network Adapter GUID: " << QString::fromWCharArray(classGuid);
            break; // 只需要获取一次即可
        }
    }

    // 清理资源
    SetupDiDestroyDeviceInfoList(hDevInfo);
}
