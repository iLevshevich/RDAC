#ifndef __UTILITY_H_INCLUDED__ 
#define __UTILITY_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Utility{
	void __fastcall to_error_event_log( const std::wstring& ); 
	void __fastcall to_info_event_log( const std::wstring& );
	void __fastcall to_warn_event_log( const std::wstring& );
}

#pragma pack( pop )

#endif //__UTILITY_H_INCLUDED__

