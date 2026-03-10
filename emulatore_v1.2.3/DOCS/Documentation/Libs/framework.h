#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Escludere gli elementi utilizzati di rado dalle intestazioni di Windows
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // alcuni costruttori CString saranno espliciti

#include <afxwin.h>         // componenti MFC di base e standard
#include <afxext.h>         // Estensioni MFC

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // classi OLE MFC
#include <afxodlgs.h>       // classi di finestre di dialogo OLE MFC
#include <afxdisp.h>        // Classi di automazione MFC
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // Classi di database ODBC MFC
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // Classi di database DAO MFC
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Supporto MFC per controlli comuni di Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Supporto MFC per controlli comuni di Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxhtml.h>
#include <afxdhtml.h>
#include <afxtabctrl.h>
#include <windows.h>
