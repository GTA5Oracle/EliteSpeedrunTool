#pragma once

#include <QList>
#include <netfw.h>
#include <windows.h>

class FirewallUtil {
public:
    FirewallUtil();

    static bool firewallIsEnabled(long& enabledTypes);

    static QList<INetFwRule*> getNetFwRule();

    static bool setNetFwRuleEnabled(bool enabled);

    static void init();

    static void release();

    static bool getIsEnabled();

private:
    static INetFwRule* getNetFwRule(NET_FW_RULE_DIRECTION direction);

    static bool setNetFwRuleEnabled(bool enabled, NET_FW_RULE_DIRECTION direction);

private:
    static bool inited;

    static bool isEnabled;

    static HRESULT hrComInit;

    static INetFwPolicy2* pNetFwPolicy2;

    static long CurrentProfilesBitMask;

    static INetFwRules* pFwRules;

    static BSTR bstrRuleInName;
    static BSTR bstrRuleOutName;

    static BSTR bstrRuleLPorts;

    static BSTR bstrRuleGroup;
};
