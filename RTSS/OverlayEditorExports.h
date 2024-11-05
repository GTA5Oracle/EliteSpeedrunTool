/////////////////////////////////////////////////////////////////////////////
//
// This header file defines functions exported by OverlayEditor.dll 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OVERLAYEDITOR_EXPORTS_INCLUDED_
#define _OVERLAYEDITOR_INCLUDED_

#ifdef OVERLAYEDITOR_EXPORTS
#define OVERLAYEDITOR_API extern "C" __declspec(dllexport)
#else
#define OVERLAYEDITOR_API extern "C" __declspec(dllimport)
#endif
 

typedef BOOL (*SENDOVERLAYMESSAGE_PROC	)(LPCSTR, LPCSTR, LPCSTR);
	//void SendOverlayMessage(LPCSTR lpMessage, LPCSTR lpLayer, LPCSTR lpParams)

#endif

