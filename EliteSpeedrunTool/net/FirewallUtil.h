#pragma once

#include "GlobalData.h"
#include "net/FirewallRule.h"
#include <QApplication>
#include <QList>
#include <netfw.h>
#include <windows.h>

#define firewallUtil (FirewallUtil::instance())

class FirewallUtil : QObject {
    Q_OBJECT
public:
    FirewallUtil();

    static FirewallUtil* instance();

    bool firewallIsEnabled(long& enabledTypes);

    QList<INetFwRule*> getNetFwRule();
    bool deleteNetFwRuleByGroup();

    bool setNetFwRuleEnabled(bool enabled);

    void init();
    void release();

    bool getIsEnabled();

private:
    void initRuleListListener();

    HRESULT initINetFwPolicy2(INetFwPolicy2** ppNetFwPolicy2);

    INetFwRule* getNetFwRule(FirewallRule* rule);
    INetFwRule* getCachedNetFwRule(FirewallRule* rule);

    QString getRuleName(FirewallRule* rule);

    BSTR getBstrRuleNames(FirewallRule* rule);

private:
    bool inited = false;

    bool isEnabled = false;

    FirewallRule defaultRule = FirewallRule("", globalData->mDefaultFirewallDirection, globalData->mDefaultFirewallProtocol);
    BSTR bstrDefaultRuleName = nullptr;

    HRESULT hrComInit = S_OK;

    INetFwPolicy2* pNetFwPolicy2 = nullptr;

    long currentProfilesBitMask = 0;

    INetFwRules* pFwRules = nullptr;

    QMap<FirewallRule*, BSTR> bstrRuleNames;

    BSTR bstrRuleLPorts;

    BSTR bstrRuleGroup;
};
