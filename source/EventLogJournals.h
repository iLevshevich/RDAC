#ifndef __EVENT_LOG_JOURNALS_H_INCLUDED__ 
#define __EVENT_LOG_JOURNALS_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Utility{

class EventLogJournals : 
				private boost::noncopyable{
	public:
		void __fastcall init( const std::string& );
		bool __fastcall isExist( const std::string&, 
							     const std::string&, 
							     const std::string&  );
		std::vector< std::string > getJournals() const;

	private:
		std::map< std::string, std::map< std::string, std::set< std::string > > >journals_options;
	};

};

#pragma pack( pop )

#endif //__EVENT_LOG_JOURNALS_H_INCLUDED__
