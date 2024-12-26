#pragma once

#include <QList>
#include <netfw.h>
#include <windows.h>

#define firewallUtil (FirewallUtil::instance())

class FirewallUtil {
public:
    FirewallUtil();

    static FirewallUtil* instance();

    bool firewallIsEnabled(long& enabledTypes);

    QList<INetFwRule*> getNetFwRule();

    bool setNetFwRuleEnabled(bool enabled);

    void init();
    void release();

    bool getIsEnabled();

private:
    HRESULT initINetFwPolicy2(INetFwPolicy2** ppNetFwPolicy2);

    INetFwRule* getNetFwRule(NET_FW_RULE_DIRECTION direction);
    INetFwRule* getCachedNetFwRule(NET_FW_RULE_DIRECTION direction);

    bool setNetFwRuleEnabled(bool enabled, NET_FW_RULE_DIRECTION direction);

private:
    bool inited = false;

    bool isEnabled = false;

    HRESULT hrComInit = S_OK;

    INetFwPolicy2* pNetFwPolicy2 = nullptr;

    long currentProfilesBitMask = 0;

    INetFwRules* pFwRules = nullptr;

    BSTR bstrRuleInName;
    BSTR bstrRuleOutName;

    BSTR bstrRuleLPorts;

    BSTR bstrRuleGroup;
};
