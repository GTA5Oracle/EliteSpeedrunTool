#include "UiAccessUtil.h"
#include <tchar.h>
#include <tlhelp32.h>

Q_GLOBAL_STATIC(UiAccessUtil, uiAccessUtilInstance)

UiAccessUtil::UiAccessUtil(QObject* parent)
    : QObject { parent }
{
}

UiAccessUtil* UiAccessUtil::instance()
{
    return uiAccessUtilInstance;
}

DWORD UiAccessUtil::prepareForUIAccess()
{
    DWORD dwErr;
    HANDLE hTokenUIAccess = nullptr;
    DWORD fUIAccess;

    if (checkForUIAccess(&dwErr, &fUIAccess)) {
        if (fUIAccess) {
            dwErr = ERROR_SUCCESS;
        } else {
            dwErr = createUIAccessToken(&hTokenUIAccess);
            if (ERROR_SUCCESS == dwErr) {
                STARTUPINFO si;
                PROCESS_INFORMATION pi;

                GetStartupInfo(&si);
                if (CreateProcessAsUser(hTokenUIAccess, nullptr, GetCommandLine(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
                    CloseHandle(pi.hProcess), CloseHandle(pi.hThread);
                    ExitProcess(0);
                } else {
                    dwErr = GetLastError();
                }

                CloseHandle(hTokenUIAccess);
            }
        }
    }

    return dwErr;
}

BOOL UiAccessUtil::checkForUIAccess(DWORD* pdwErr, DWORD* pfUIAccess)
{
    BOOL result = FALSE;
    HANDLE hToken;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        DWORD dwRetLen;

        if (GetTokenInformation(hToken, TokenUIAccess, pfUIAccess, sizeof(*pfUIAccess), &dwRetLen)) {
            result = TRUE;
        } else {
            *pdwErr = GetLastError();
        }
        CloseHandle(hToken);
    } else {
        *pdwErr = GetLastError();
    }

    return result;
}

DWORD UiAccessUtil::duplicateWinloginToken(DWORD dwSessionId, DWORD dwDesiredAccess, PHANDLE phToken)
{
    DWORD dwErr;
    PRIVILEGE_SET ps;

    ps.PrivilegeCount = 1;
    ps.Control = PRIVILEGE_SET_ALL_NECESSARY;

    if (LookupPrivilegeValue(nullptr, SE_TCB_NAME, &ps.Privilege[0].Luid)) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE != hSnapshot) {
            BOOL bCont, bFound = FALSE;
            PROCESSENTRY32 pe;

            pe.dwSize = sizeof(pe);
            dwErr = ERROR_NOT_FOUND;

            for (bCont = Process32First(hSnapshot, &pe); bCont; bCont = Process32Next(hSnapshot, &pe)) {
                HANDLE hProcess;

                if (0 != _tcsicmp(pe.szExeFile, TEXT("winlogon.exe"))) {
                    continue;
                }

                hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
                if (hProcess) {
                    HANDLE hToken;
                    DWORD dwRetLen, sid;

                    if (OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_DUPLICATE, &hToken)) {
                        BOOL fTcb;

                        if (PrivilegeCheck(hToken, &ps, &fTcb) && fTcb) {
                            if (GetTokenInformation(hToken, TokenSessionId, &sid, sizeof(sid), &dwRetLen) && sid == dwSessionId) {
                                bFound = TRUE;
                                if (DuplicateTokenEx(hToken, dwDesiredAccess, nullptr, SecurityImpersonation, TokenImpersonation, phToken)) {
                                    dwErr = ERROR_SUCCESS;
                                } else {
                                    dwErr = GetLastError();
                                }
                            }
                        }
                        CloseHandle(hToken);
                    }
                    CloseHandle(hProcess);
                }

                if (bFound)
                    break;
            }

            CloseHandle(hSnapshot);
        } else {
            dwErr = GetLastError();
        }
    } else {
        dwErr = GetLastError();
    }

    return dwErr;
}

DWORD UiAccessUtil::createUIAccessToken(PHANDLE phToken)
{
    DWORD dwErr;
    HANDLE hTokenSelf;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hTokenSelf)) {
        DWORD dwSessionId, dwRetLen;

        if (GetTokenInformation(hTokenSelf, TokenSessionId, &dwSessionId, sizeof(dwSessionId), &dwRetLen)) {
            HANDLE hTokenSystem = nullptr;

            dwErr = duplicateWinloginToken(dwSessionId, TOKEN_IMPERSONATE, &hTokenSystem);
            if (ERROR_SUCCESS == dwErr) {
                if (SetThreadToken(nullptr, hTokenSystem)) {
                    if (DuplicateTokenEx(hTokenSelf, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_DEFAULT, nullptr, SecurityAnonymous, TokenPrimary, phToken)) {
                        BOOL bUIAccess = TRUE;

                        if (!SetTokenInformation(*phToken, TokenUIAccess, &bUIAccess, sizeof(bUIAccess))) {
                            dwErr = GetLastError();
                            CloseHandle(*phToken);
                        }
                    } else {
                        dwErr = GetLastError();
                    }
                    RevertToSelf();
                } else {
                    dwErr = GetLastError();
                }
                CloseHandle(hTokenSystem);
            }
        } else {
            dwErr = GetLastError();
        }

        CloseHandle(hTokenSelf);
    } else {
        dwErr = GetLastError();
    }

    return dwErr;
}
