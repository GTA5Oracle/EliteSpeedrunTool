#pragma once

#include <netfw.h>
#include <windows.h>

class FirewallUtil {
public:
    FirewallUtil();

    static bool firewallIsEnabled();

    static INetFwRule* getNetFwRule();

    static bool setNetFwRuleEnabled(bool enabled);

    static void init();

    static void release();

    static bool getIsEnabled();

private:
    static bool inited;

    static bool isEnabled;

    static HRESULT hrComInit;

    static INetFwPolicy2* pNetFwPolicy2;

    static long CurrentProfilesBitMask;

    static INetFwRules* pFwRules;

    static BSTR bstrRuleName;

    static BSTR bstrRuleLPorts;

    static BSTR bstrRuleGroup;
};
