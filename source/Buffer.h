#ifndef __BUFFER_H_INCLUDED__ 
#define __BUFFER_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Memmory{

	template< class T >
	class Buffer: 
			private boost::noncopyable{
	public:
		typedef T BaseType;
		typedef T* PointerType;

		Buffer( const std::size_t size ): 
								size_( size ), 
								buffer_( new BaseType[ size ] ){
			memset( buffer_.get(), 0, size );   
		}

		PointerType get()const {
			return buffer_.get();
		}

		const std::size_t size() const{
			return size_;
		}

		void set( PointerType const buffer_ptr, const std::size_t size ){
			buffer_.reset( buffer_ptr );
			size_ = size;
		}

	private:
		boost::scoped_array< BaseType > buffer_;
		std::size_t size_;
	};

};

#pragma pack( pop )

#endif //__BUFFER_H_INCLUDED__