#ifndef __STD_AFX_H_INCLUDED__ 
#define __STD_AFX_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#if !defined(__TRACERT__)
//#	define __TRACERT__
#endif

#if !defined(__ISO_DATE_TIME_FORMAT__)
#	define __ISO_DATE_TIME_FORMAT__
#endif

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				
#define WINVER NTDDI_WIN7		
#endif		

#ifndef _WIN32_WINNT	
#define _WIN32_WINNT NTDDI_WIN7
#endif	

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS
#define _ATL_NO_COM_SUPPORT

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
using namespace ATL;

#ifndef __MODULE_NAME__
#	define __MODULE_NAME__ L"RDAC"
#endif

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <clocale>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <algorithm>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/scoped_array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/locale/util.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/system/error_code.hpp> 

#include <windows.h>
#include <comutil.h>
#include <shellapi.h>

#pragma comment( lib, "comsuppw" )
#pragma comment( lib, "advapi32" )

#endif //__STD_AFX_H_INCLUDED__

/*
OS                             DATE               WINVER=

Windows 1.x:
Windows 1.01         (11/15/85)
Windows 1.02
Windows 1.03            (8//86)
Windows 1.04            (4//87)

Windows 2.x:
Windows 2.0     
Windows 2.03           (11//87)
Windows/286 2.10      (5/27/88)
Windows/386 2.10
Windows 2.11          (3/13/89)

Windows 3.x:
Windows 3.0              (5/22/90)   WINVER=0x0300
Windows 3.0a          (10/31/90)
Windows 3.0a MME         (1991)
Windows 3.1            (4/6/92)      WINVER=0x030a
Windows 3.11         (12/31/93)
Windows 3.2  [VR China]  (1994)

Windows for Workgroups 3.1
Windows for Workgroups 3.11

Windows NT 3.1     _WIN32_WINNT=0x030A
(Win32s                  _WIN32_WINNT=0x0)
Windows NT 3.5
Windows NT 3.51


Windows 95 and Windows NT 4.0         WINVER=0x0400
Windows NT 4.0     [only, no Win95]      WINVER=0x0400 and _WIN32_WINNT=0x0400
Windows 95           [only, no WinNT]      WINVER=0x0400 and _WIN32_WINDOWS=0x0400
Windows 98 and Windows NT 4.0         WINVER=0x0400 and _WIN32_WINDOWS=0x0410

Windows 98 WINVER=0x0400 and _WIN32_WINDOWS=0x0410
Windows ME WINVER=0x0400 and _WIN32_WINDOWS=0x0490


Minimum version required    Minimum value of _WIN32_IE
Internet Explorer 8.0    _WIN32_IE_IE80 (0x0800)
Internet Explorer 7.0    _WIN32_IE_IE70 (0x0700)
Internet Explorer 6.0 SP2    _WIN32_IE_IE60SP2 (0x0603)
Internet Explorer 6.0 SP1    _WIN32_IE_IE60SP1 (0x0601)
Internet Explorer 6.0    _WIN32_IE_IE60 (0x0600)
Internet Explorer 5.5    _WIN32_IE_IE55 (0x0550)
Internet Explorer 5.01    _WIN32_IE_IE501 (0x0501)
Internet Explorer 5.0, 5.0a, 5.0b    _WIN32_IE_IE50 (0x0500)
Internet Explorer 4.01 _WIN32_IE >= 0x0401
Internet Explorer 4.0  _WIN32_IE >= 0x0400
Internet Explorer 3.0, 3.01, 3.02 _WIN32_IE >= 0x0300

Minimum system required    Value for NTDDI_VERSION
Windows 7    NTDDI_WIN7 (0x06010000)
Windows Server 2008    NTDDI_WS08 (0x06000100)
Windows Vista with Service Pack 1 (SP1)    NTDDI_VISTASP1 (0x06000100)
Windows Vista    NTDDI_VISTA (0x06000000)
Windows Server 2003 with Service Pack 2 (SP2)    NTDDI_WS03SP2 (0x05020200)
Windows Server 2003 with Service Pack 1 (SP1)    NTDDI_WS03SP1 (0x05020100)
Windows Server 2003    NTDDI_WS03 (0x05020000)
Windows XP with Service Pack 3 (SP3)    NTDDI_WINXPSP3 (0x05010300)
Windows XP with Service Pack 2 (SP2)    NTDDI_WINXPSP2 (0x05010200)
Windows XP with Service Pack 1 (SP1)    NTDDI_WINXPSP1 (0x05010100)
Windows XP    NTDDI_WINXP (0x05010000)

Minimum system required    Minimum value for _WIN32_WINNT and WINVER
Windows 7    _WIN32_WINNT_WIN7 (0x0601)
Windows Server 2008    _WIN32_WINNT_WS08 (0x0600)
Windows Vista    _WIN32_WINNT_VISTA (0x0600)
Windows Server 2003 with SP1, Windows XP with SP2    _WIN32_WINNT_WS03 (0x0502)
Windows Server 2003, Windows XP    _WIN32_WINNT_WINXP (0x0501)
Windows 2000 _WIN32_WINNT=0x0500 and WINVER=0x0500
*/