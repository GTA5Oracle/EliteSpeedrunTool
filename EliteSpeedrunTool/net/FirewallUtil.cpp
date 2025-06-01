#include "FirewallUtil.h"
#include "NumberUtil.h"

#include <QtConcurrent>
#include <comdef.h>

Q_GLOBAL_STATIC(FirewallUtil, firewallUtilInstance)

FirewallUtil::FirewallUtil()
{
    initRuleListListener();
}

FirewallUtil* FirewallUtil::instance()
{
    return firewallUtilInstance;
}

bool FirewallUtil::firewallIsEnabled(long& enabledTypes)
{
    bool enabled = false;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        qCritical() << "CoInitializeEx failed: " << hr;
        return false;
    }

    INetFwPolicy2* pNetFwPolicy2 = NULL;
    hr = CoCreateInstance(
        __uuidof(NetFwPolicy2),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwPolicy2),
        (void**)&pNetFwPolicy2);

    if (FAILED(hr)) {
        qCritical() << "CoCreateInstance failed: " << hr;
        CoUninitialize();
        return false;
    }

    VARIANT_BOOL bFirewallEnabled;
    long fwCurrentProfileTypes;
    pNetFwPolicy2->get_CurrentProfileTypes(&fwCurrentProfileTypes);
    enabledTypes = 0;
    NET_FW_PROFILE_TYPE2 profileTypes[] = { NET_FW_PROFILE2_DOMAIN, NET_FW_PROFILE2_PRIVATE, NET_FW_PROFILE2_PUBLIC };
    for (NET_FW_PROFILE_TYPE2 type : profileTypes) {
        hr = pNetFwPolicy2->get_FirewallEnabled((NET_FW_PROFILE_TYPE2)(fwCurrentProfileTypes & type), &bFirewallEnabled);
        bool currentEnabled = false;

        if (SUCCEEDED(hr)) {
            currentEnabled = bFirewallEnabled == VARIANT_TRUE;
        } else {
            currentEnabled = false;
            qCritical() << "Failed to get firewall status: " << hr << " type: " << type;
        }

        enabled |= currentEnabled;

        if (currentEnabled) {
            qInfo() << "NET_FW_PROFILE_TYPE2: " << type << " enabled";
            enabledTypes |= type;
        } else {
            qCritical() << "NET_FW_PROFILE_TYPE2: " << type << " disabled";
            if (type == NET_FW_PROFILE2_PUBLIC) {
                qCritical() << "NET_FW_PROFILE2_PUBLIC disabled!!!";
            }
        }
    }

    pNetFwPolicy2->Release();
    CoUninitialize();

    return enabled;
}

// Instantiate INetFwPolicy2
HRESULT FirewallUtil::initINetFwPolicy2(INetFwPolicy2** ppNetFwPolicy2)
{
    HRESULT hr = S_OK;

    hr = CoCreateInstance(
        __uuidof(NetFwPolicy2),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwPolicy2),
        (void**)ppNetFwPolicy2);

    if (FAILED(hr)) {
        qCritical("CoCreateInstance for INetFwPolicy2 failed: %ld", hr);
        goto Cleanup;
    }

Cleanup:
    return hr;
}

INetFwRule* FirewallUtil::getNetFwRule(FirewallRule* rule)
{
    if (!inited)
        return nullptr;

    HRESULT hr = S_OK;

    INetFwRule* pFwRule = getCachedNetFwRule(rule);
    if (pFwRule) {
        return pFwRule;
    }

    // Create a new Firewall Rule object.
    hr = CoCreateInstance(
        __uuidof(NetFwRule),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwRule),
        (void**)&pFwRule);
    if (FAILED(hr)) {
        qCritical("CoCreateInstance for Firewall Rule failed: %ld", hr);
        goto Cleanup;
    }

    // Populate the Firewall Rule object
    pFwRule->put_Name(getBstrRuleNames(rule));
    if (!rule->path.isEmpty()) {
        pFwRule->put_ApplicationName(SysAllocString(
            QString(rule->path)
                .replace("/", "\\")
                .toStdWString()
                .c_str()));
    }
    {
        int protocol = rule->protocol;
        if (protocol >= 0 && protocol <= 255) {
            pFwRule->put_Protocol(protocol);
        }
    }
    //    pFwRule->put_LocalPorts(bstrRuleLPorts);
    pFwRule->put_Grouping(bstrRuleGroup);
    pFwRule->put_Direction(static_cast<NET_FW_RULE_DIRECTION>(rule->direction));
    //    pFwRule->put_Profiles(CurrentProfilesBitMask);
    pFwRule->put_Action(NET_FW_ACTION_BLOCK);
    pFwRule->put_Enabled(VARIANT_TRUE);

    // Add the Firewall Rule
    hr = pFwRules->Add(pFwRule);
    if (FAILED(hr)) {
        qCritical("Firewall Rule Add failed: %ld", hr);
        goto Cleanup;
    }

    return pFwRule;

Cleanup:
    if (pFwRule != NULL) {
        pFwRule->Release();
    }

    return nullptr;
}

INetFwRule* FirewallUtil::getCachedNetFwRule(FirewallRule* rule)
{
    INetFwRule* pFwRule = nullptr;
    HRESULT hr = S_OK;
    hr = pFwRules->Item(getBstrRuleNames(rule), (INetFwRule**)&pFwRule);
    if (!SUCCEEDED(hr)) {
        return nullptr;
    }
    if (globalData->debugMode()) {
        qInfo("Found NetFwRule by pFwRules->Item()");
    }
    auto newPath = SysAllocString(
        QString(rule->path)
            .replace("/", "\\")
            .toStdWString()
            .c_str());
    if (rule->path.isEmpty()) {
        hr = pFwRule->put_ApplicationName(NULL);
    } else {
        BSTR path;
        if (FAILED(pFwRule->get_ApplicationName(&path)) || path != newPath) {
            hr = pFwRule->put_ApplicationName(newPath);
        } else {
            hr = S_OK;
        }
        SysFreeString(newPath);
    }
    if (FAILED(hr)) {
        qCritical("put_ApplicationName failed! (%ld)", hr);
        return nullptr;
    }

    auto newDirection = static_cast<NET_FW_RULE_DIRECTION>(rule->direction);
    NET_FW_RULE_DIRECTION direction;
    if (FAILED(pFwRule->get_Direction(&direction)) || direction != newDirection) {
        hr = pFwRule->put_Direction(newDirection);
        if (FAILED(hr)) {
            qCritical("put_Direction failed! (%ld)", hr);
            return nullptr;
        }
    }

    LONG newProtocol = rule->protocol;
    if (newProtocol < 0 || newProtocol > 256) {
        newProtocol = 256;
    }
    LONG protocol;
    if (FAILED(pFwRule->get_Protocol(&protocol)) || protocol != newProtocol) {
        hr = pFwRule->put_Protocol(newProtocol);
        if (FAILED(hr)) {
            qCritical("put_Protocol failed! (%ld)", hr);
        }
    }
    return pFwRule;
}

QString FirewallUtil::getRuleName(FirewallRule* rule)
{
    return QApplication::applicationName() + " - "
        + rule->path.toUtf8().toBase64() + " - "
        + QString::number(rule->direction) + " - "
        + QString::number(rule->protocol);
}

BSTR FirewallUtil::getBstrRuleNames(FirewallRule* rule)
{
    if (rule == &defaultRule) {
        if (!bstrDefaultRuleName) {
            bstrDefaultRuleName = SysAllocString(getRuleName(&defaultRule).toStdWString().c_str());
        }
        return bstrDefaultRuleName;
    }
    return bstrRuleNames[rule];
}

QList<INetFwRule*> FirewallUtil::getNetFwRule()
{
    QList<INetFwRule*> result;
    if (globalData->firewallRuleList().isEmpty()) {
        result << getNetFwRule(&defaultRule);
    } else {
        for (auto& path : globalData->firewallRuleList()) {
            result << getNetFwRule(path);
        }
    }
    return result;
}

bool FirewallUtil::deleteNetFwRuleByGroup()
{
    QList<INetFwRule*> result;
    HRESULT hr = S_OK;
    IEnumVARIANT* pEnumerator = nullptr;
    hr = pFwRules->get__NewEnum(reinterpret_cast<IUnknown**>(&pEnumerator));
    if (FAILED(hr)) {
        wprintf(L"get__NewEnum failed: 0x%08lx\n", hr);
        pFwRules->Release();
        pNetFwPolicy2->Release();
        CoUninitialize();
        return false;
    }

    VARIANT var;
    ULONG cFetched = 0;

    while (SUCCEEDED(pEnumerator->Next(1, &var, &cFetched)) && cFetched == 1) {
        INetFwRule* pFwRule = nullptr;
        IDispatch* pDispatch = var.pdispVal;
        hr = pDispatch->QueryInterface(__uuidof(INetFwRule), (void**)&pFwRule);
        if (SUCCEEDED(hr)) {
            BSTR bstrGroup = nullptr;
            hr = pFwRule->get_Grouping(&bstrGroup);
            if (SUCCEEDED(hr) && bstrGroup != nullptr) {
                if (_wcsicmp(bstrGroup, bstrRuleGroup) == 0) { // 不区分大小写比较
                    result << pFwRule;
                    BSTR bstrName = nullptr;
                    hr = pFwRule->get_Name(&bstrName);
                    if (SUCCEEDED(hr) && bstrName != nullptr) {
                        hr = pFwRules->Remove(bstrName);
                        if (SUCCEEDED(hr)) {
                            wprintf(L"Successfully deleted rule: %s\n", bstrName);
                        } else {
                            wprintf(L"Failed to delete rule: %s (0x%08lx)\n", bstrName, hr);
                        }
                        SysFreeString(bstrName);
                    }
                }
                SysFreeString(bstrGroup);
            }
            pFwRule->Release();
        }
        VariantClear(&var);
    }

    pEnumerator->Release();

    return true;
}

bool FirewallUtil::setNetFwRuleEnabled(bool enabled)
{
    QList<INetFwRule*> rules;
    if (globalData->firewallRuleList().isEmpty()) {
        rules << getNetFwRule(&defaultRule);
    } else {
        for (auto rule : globalData->firewallRuleList()) {
            rules << getNetFwRule(rule);
        }
    }
    if (rules.isEmpty()) {
        qCritical("rules is empty!!");
        qCritical("Firewall operate failed!");
        return false;
    }
    QList<QFuture<HRESULT>> futures = {};
    for (const auto& rule : rules) {
        if (rule) {
            futures += QtConcurrent ::run(&INetFwRule::put_Enabled, rule, enabled ? VARIANT_TRUE : VARIANT_FALSE);
        } else {
            qWarning() << "setNetFwRuleEnabled(bool enabled): rule is null";
        }
    }

    for (auto& future : futures) {
        if (FAILED((future.result()))) {
            qCritical("setNetFwRuleEnabled rule->put_Enabled(%s) failed!",
                (enabled ? "VARIANT_TRUE" : "VARIANT_FALSE"));
            qCritical("Firewall operate failed!");
        }
    }

    if (enabled) {
        qInfo("Firewall successfully enabled!");
    } else {
        qInfo("Firewall successfully disabled!");
    }
    isEnabled = enabled;
    return true;
}

void FirewallUtil::init()
{
    if (inited)
        return;

    qInfo("Initializing the firewall...");
    hrComInit = S_OK;
    //    bstrRuleLPorts = SysAllocString(L"80");
    bstrRuleGroup = SysAllocString(L"AutoFirewall");

    HRESULT hr = S_OK;

    // Initialize COM.
    hrComInit = CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    // Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
    // initialized with a different mode. Since we don't care what the mode is,
    // we'll just use the existing mode.
    if (hrComInit != RPC_E_CHANGED_MODE) {
        if (FAILED(hrComInit)) {
            qCritical("CoInitializeEx failed: %ld", hrComInit);
            release();
            return;
        }
    }

    // Retrieve INetFwPolicy2
    hr = initINetFwPolicy2(&pNetFwPolicy2);
    if (FAILED(hr)) {
        qCritical("WFCOMInitialize failed!");
        release();
        return;
    }

    // Retrieve INetFwRules
    hr = pNetFwPolicy2->get_Rules(&pFwRules);
    if (FAILED(hr)) {
        qCritical("get_Rules failed: %ld", hr);
        release();
        return;
    }

    // Retrieve Current Profiles bitmask
    hr = pNetFwPolicy2->get_CurrentProfileTypes(&currentProfilesBitMask);
    if (FAILED(hr)) {
        qCritical("get_CurrentProfileTypes failed: %ld", hr);
        release();
        return;
    }

    QList<INetFwRule*> fwRules = getNetFwRule();
    int allEnabled = 0;
    for (auto rule : fwRules) {
        if (rule) {
            VARIANT_BOOL enabled;
            rule->get_Enabled(&enabled);
            allEnabled = (allEnabled << 1) | (enabled == VARIANT_TRUE);
        } else {
            qInfo("auto fwRule = getNetFwRule(); fwRule is null");
        }
    }
    if (allEnabled == 0) {
        isEnabled = false;
    } else {
        if (fwRules.size() == NumberUtil::bitCount(allEnabled)) {
            isEnabled = true;
        } else {
            qCritical("Not all firewall rules have same enabled");
            setNetFwRuleEnabled(false);
            isEnabled = false;
        }
    }

    inited = true;
    CoUninitialize();
    qInfo("Initializing the firewall successfully");
}

void FirewallUtil::release()
{
    if (!inited)
        return;

    deleteNetFwRuleByGroup();

    // Free BSTR's
    SysFreeString(bstrRuleLPorts);
    SysFreeString(bstrDefaultRuleName);

    // Release the INetFwRules object
    if (pFwRules != NULL) {
        pFwRules->Release();
    }

    // Release the INetFwPolicy2 object
    if (pNetFwPolicy2 != NULL) {
        pNetFwPolicy2->Release();
    }

    // Uninitialize COM.
    if (SUCCEEDED(hrComInit)) {
        CoUninitialize();
    }
}

bool FirewallUtil::getIsEnabled()
{
    return isEnabled;
}

void FirewallUtil::initRuleListListener()
{
    auto callBlock = [this]() {
        for (auto& bstr : bstrRuleNames) {
            SysFreeString(bstr);
        }
        bstrRuleNames.clear();
        for (auto& rule : globalData->firewallRuleList()) {
            bstrRuleNames.insert(rule, SysAllocString(getRuleName(rule).toStdWString().c_str()));
        }
    };
    connect(globalData, &GlobalData::firewallRuleListChanged, this, [callBlock]() { callBlock(); });
    callBlock();
}
