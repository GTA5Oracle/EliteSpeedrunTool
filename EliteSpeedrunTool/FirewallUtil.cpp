#include "FirewallUtil.h"
#include "GlobalData.h"

bool FirewallUtil::inited = false;
bool FirewallUtil::isEnabled = false;
HRESULT FirewallUtil::hrComInit = S_OK;
INetFwPolicy2* FirewallUtil::pNetFwPolicy2 = nullptr;
long FirewallUtil::CurrentProfilesBitMask = 0;
INetFwRules* FirewallUtil::pFwRules = nullptr;
BSTR FirewallUtil::bstrRuleName;
BSTR FirewallUtil::bstrRuleLPorts;
BSTR FirewallUtil::bstrRuleGroup;

FirewallUtil::FirewallUtil()
{
}

bool FirewallUtil::firewallIsEnabled()
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
    hr = pNetFwPolicy2->get_FirewallEnabled((NET_FW_PROFILE_TYPE2_)fwCurrentProfileTypes, &bFirewallEnabled);

    if (SUCCEEDED(hr)) {
        enabled = bFirewallEnabled == VARIANT_TRUE;
    } else {
        enabled = false;
        qCritical() << "Failed to get firewall status: " << hr;
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

INetFwRule* FirewallUtil::getNetFwRule()
{
    if (!inited)
        return nullptr;

    HRESULT hr = S_OK;

    INetFwRule* pFwRule = NULL;

    hr = pFwRules->Item(bstrRuleName, (INetFwRule**)&pFwRule);
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
        h2 = pFwRule->put_Direction(static_cast<NET_FW_RULE_DIRECTION>(globalData->firewallDirection()));
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
    pFwRule->put_Name(bstrRuleName);
    if (!globalData->firewallAppPath().isEmpty()) {
        pFwRule->put_ApplicationName(SysAllocString(
            QString(globalData->firewallAppPath())
                .replace("/", "\\")
                .toStdWString()
                .c_str()));
    }
    pFwRule->put_Protocol(NET_FW_IP_PROTOCOL_TCP);
    //    pFwRule->put_LocalPorts(bstrRuleLPorts);
    pFwRule->put_Grouping(bstrRuleGroup);
    pFwRule->put_Direction(static_cast<NET_FW_RULE_DIRECTION>(globalData->firewallDirection()));
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

bool FirewallUtil::setNetFwRuleEnabled(bool enabled)
{
    if (enabled == isEnabled)
        return true;
    INetFwRule* fwRule = getNetFwRule();
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
    isEnabled = enabled;

    if (enabled) {
        qInfo("Firewall successfully enabled!");
    } else {
        qInfo("Firewall successfully disabled!");
    }
    return true;
}

void FirewallUtil::init()
{
    if (inited)
        return;

    qInfo("Initializing the firewall...");
    hrComInit = S_OK;
    bstrRuleName = SysAllocString(L"AutoFirewall");
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

    // When possible we avoid adding firewall rules to the Public profile.
    // If Public is currently active and it is not the only active profile, we remove it from the bitmask
    if ((CurrentProfilesBitMask & NET_FW_PROFILE2_PUBLIC)
        && (CurrentProfilesBitMask != NET_FW_PROFILE2_PUBLIC)) {
        CurrentProfilesBitMask ^= NET_FW_PROFILE2_PUBLIC;
    }

    auto fwRule = getNetFwRule();
    if (fwRule) {
        VARIANT_BOOL enabled;
        fwRule->get_Enabled(&enabled);
        isEnabled = (enabled == VARIANT_TRUE);
    } else {
        qInfo("auto fwRule = getNetFwRule(); fwRule is null");
    }

    inited = true;

    qInfo("Initializing the firewall successfully");
}

void FirewallUtil::release()
{
    if (!inited)
        return;

    // Free BSTR's
    SysFreeString(bstrRuleName);
    SysFreeString(bstrRuleLPorts);

    INetFwRule* fwRule = getNetFwRule();
    if (fwRule != NULL) {
        BSTR name = nullptr;
        fwRule->get_Name(&name);
        pFwRules->Remove(name);
        fwRule->Release();
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
