#ifndef __OPTIONS_H_INCLUDED__ 
#define __OPTIONS_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Utility{

	class Options : 
				private boost::noncopyable{
	public:
		void __fastcall init( int argc, _TCHAR* argv[] );
		std::string __fastcall get( const std::string& ) const;

	private:
		boost::program_options::variables_map vm;
	};

};

#pragma pack( pop )

#endif //__OPTIONS_H_INCLUDED__

