#include "FirewallUtil.h"
#include "GlobalData.h"
#include "NumberUtil.h"

bool FirewallUtil::inited = false;
bool FirewallUtil::isEnabled = false;
HRESULT FirewallUtil::hrComInit = S_OK;
INetFwPolicy2* FirewallUtil::pNetFwPolicy2 = nullptr;
long FirewallUtil::CurrentProfilesBitMask = 0;
INetFwRules* FirewallUtil::pFwRules = nullptr;
BSTR FirewallUtil::bstrRuleInName;
BSTR FirewallUtil::bstrRuleOutName;
BSTR FirewallUtil::bstrRuleLPorts;
BSTR FirewallUtil::bstrRuleGroup;

FirewallUtil::FirewallUtil()
{
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
HRESULT WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2)
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

INetFwRule* FirewallUtil::getNetFwRule(NET_FW_RULE_DIRECTION direction)
{
    if (!inited)
        return nullptr;

    HRESULT hr = S_OK;

    INetFwRule* pFwRule = NULL;

    hr = pFwRules->Item(direction == NET_FW_RULE_DIR_IN ? bstrRuleInName : bstrRuleOutName, (INetFwRule**)&pFwRule);
    if (SUCCEEDED(hr)) {
        qInfo("Found NetFwRule by pFwRules->Item()");
        HRESULT h, h2;
        if (globalData->firewallAppPath().isEmpty()) {
            h = pFwRule->put_ApplicationName(NULL);
        } else {
            h = pFwRule->put_ApplicationName(SysAllocString(
                QString(globalData->firewallAppPath())
                    .replace("/", "\\")
                    .toStdWString()
                    .c_str()));
        }
        h2 = pFwRule->put_Direction(direction);
        if (!SUCCEEDED(h)) {
            qCritical("put_ApplicationName failed! (%ld)", h);
        } else if (!SUCCEEDED(h2)) {
            qCritical("put_Direction failed! (%ld)", h);
        } else {
            return pFwRule;
        }
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
    pFwRule->put_Name(direction == NET_FW_RULE_DIR_IN ? bstrRuleInName : bstrRuleOutName);
    if (!globalData->firewallAppPath().isEmpty()) {
        pFwRule->put_ApplicationName(SysAllocString(
            QString(globalData->firewallAppPath())
                .replace("/", "\\")
                .toStdWString()
                .c_str()));
    }
    // pFwRule->put_Protocol(NET_FW_IP_PROTOCOL_TCP);
    //    pFwRule->put_LocalPorts(bstrRuleLPorts);
    pFwRule->put_Grouping(bstrRuleGroup);
    pFwRule->put_Direction(direction);
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

QList<INetFwRule*> FirewallUtil::getNetFwRule()
{
    int dir = globalData->firewallDirection();
    if (dir == 0) {
        return { getNetFwRule(NET_FW_RULE_DIR_IN), getNetFwRule(NET_FW_RULE_DIR_OUT) };
    } else {
        return { getNetFwRule(static_cast<NET_FW_RULE_DIRECTION_>(dir)) };
    }
}

bool FirewallUtil::setNetFwRuleEnabled(bool enabled, NET_FW_RULE_DIRECTION direction)
{
    INetFwRule* fwRule = getNetFwRule(direction);
    if (!fwRule) {
        qCritical("getNetFwRule() return null!");
        qCritical("Firewall operate failed!");
        return false;
    }
    if (FAILED(fwRule->put_Enabled(enabled ? VARIANT_TRUE : VARIANT_FALSE))) {
        qCritical("setNetFwRuleEnabled fwRule->put_Enabled(%s) failed!",
            (enabled ? "VARIANT_TRUE" : "VARIANT_FALSE"));
        qCritical("Firewall operate failed!");
        return false;
    }

    if (enabled) {
        qInfo("Firewall successfully enabled!");
    } else {
        qInfo("Firewall successfully disabled!");
    }
    return true;
}

bool FirewallUtil::setNetFwRuleEnabled(bool enabled)
{
    int dir = globalData->firewallDirection();
    if (dir == 0) {
        bool in = setNetFwRuleEnabled(enabled, NET_FW_RULE_DIR_IN);
        bool out = setNetFwRuleEnabled(enabled, NET_FW_RULE_DIR_OUT);
        if (in && out) {
            isEnabled = enabled;
        } else {
            setNetFwRuleEnabled(!enabled, NET_FW_RULE_DIR_IN);
            setNetFwRuleEnabled(!enabled, NET_FW_RULE_DIR_OUT);
            isEnabled = !enabled;
        }
        return in && out;
    } else {
        bool result = setNetFwRuleEnabled(enabled, static_cast<NET_FW_RULE_DIRECTION_>(dir));
        isEnabled = result ? enabled : !enabled;
        return result;
    }
}

void FirewallUtil::init()
{
    if (inited)
        return;

    qInfo("Initializing the firewall...");
    hrComInit = S_OK;
    bstrRuleInName = SysAllocString(L"AutoFirewallIn");
    bstrRuleOutName = SysAllocString(L"AutoFirewallOut");
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
    hr = WFCOMInitialize(&pNetFwPolicy2);
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
    hr = pNetFwPolicy2->get_CurrentProfileTypes(&CurrentProfilesBitMask);
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

    qInfo("Initializing the firewall successfully");
}

void FirewallUtil::release()
{
    if (!inited)
        return;

    // Free BSTR's
    SysFreeString(bstrRuleInName);
    SysFreeString(bstrRuleOutName);
    SysFreeString(bstrRuleLPorts);

    QList<INetFwRule*> fwRules = getNetFwRule();
    for (auto rule : fwRules) {
        if (rule) {
            BSTR name = nullptr;
            rule->get_Name(&name);
            pFwRules->Remove(name);
            rule->Release();
        }
    }

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
