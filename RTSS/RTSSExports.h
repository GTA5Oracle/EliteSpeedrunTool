/////////////////////////////////////////////////////////////////////////////
//
// This header file defines functions exported by RTSS.exe 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _RTSS_EXPORTS_INCLUDED_
#define _RTSS_EXPORTS_INCLUDED_

#ifdef RTSS_EXPORTS
#define RTSS_API extern "C" __declspec(dllexport)
#else
#define RTSS_API extern "C" __declspec(dllimport)
#endif
 
/////////////////////////////////////////////////////////////////////////////
//
// host application IDs
//
/////////////////////////////////////////////////////////////////////////////
#define RTSS_HOST_APP					'RTSS'
/////////////////////////////////////////////////////////////////////////////
//
// exported function names
//
/////////////////////////////////////////////////////////////////////////////
#define HOST_APP_PROPERTY_SKIN_COLOR_HEADER_BGND				0x0000C000
#define HOST_APP_PROPERTY_SKIN_COLOR_HEADER_TEXT				0x0000C001
/////////////////////////////////////////////////////////////////////////////
//R/O access to host application ID/version and properties

typedef DWORD (*GET_HOST_APP_ID_PROC		)();
	//DWORD GetHostAppID()
typedef DWORD (*GET_HOST_APP_VERSION_PROC	)();
	//DWORD GetHostAppVersion()
typedef BOOL  (*GET_HOST_APP_PROPERTY_PROC	)(DWORD, LPVOID, DWORD);
	//BOOL GetHostAppPropperty(DWORD dwProperty, LPVOID lpData, DWORD dwSize)

//localization

typedef void	(*LOCALIZEWND_PROC	)(HWND);
	//void LocalizeWnd(HWND hWnd)
typedef void	(*LOCALIZEMENU_PROC	)(HMENU);
	//void LocalizeWnd(HMENU hMenu)
typedef LPCSTR	(*LOCALIZESTR_PROC	)(LPCSTR);
	//LPCSTR LocalizeStr(LPCSTR lpStr)

//color picker

#define PICKCOLOREX_FLAG_MORE_BUTTTON							0x00000001

typedef DWORD	(*PICKCOLOR_PROC		)(HWND, DWORD, LPDWORD);
	//DWORD PickColor(HWND hWnd, DWORD dwColor, LPDWORD lpRecentColors);
typedef INT_PTR	(*PICKCOLOREX_PROC		)(HWND, DWORD, LPDWORD, DWORD, LPDWORD);
	//INT_PTR PickColorEx(HWND hWnd, DWORD lpColor, LPDWORD lpRecentColors, DWORD dwFlags, LPDWORD lpResult);
typedef BOOL	(*GETCOLORPREVIEW_PROC	)(DWORD dwColor, DWORD dwWidth, DWORD dwHeight, DWORD dwSize, LPBYTE lpImage);
	//BOOL GetColorPreview(DWORD dwColor, DWORD dwWidth, DWORD dwHeight, DWORD dwSize, LPBYTE lpImage);

#endif

