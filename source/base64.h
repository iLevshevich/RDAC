#ifndef __BASE64_H_INCLUDED__ 
#define __BASE64_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Http
{
	namespace base64
	{
		std::string base64_encode( unsigned char const* );
		std::string base64_decode( std::string const& );
	}
}

#pragma pack( pop )

#endif //__BASE64_H_INCLUDED__
