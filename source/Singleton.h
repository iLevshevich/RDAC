#ifndef __SINGLETON_H_INCLUDED__ 
#define __SINGLETON_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Memmory{
	
	template< class T >
	class Singleton: 
				private boost::noncopyable{
	public:
			static T& Instance() {
					static T instance;
					return instance;
			}
	};

};

#pragma pack( pop )

#endif //__SINGLETON_H_INCLUDED__