#ifndef __FWDHEADERS_H_INCLUDED__ 
#define __FWDHEADERS_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace EntryPoint{
	class EventLog;
};

namespace Memmory{
	template< class T > class Buffer;
	template< class T > class Singleton;
};

namespace Command{
	class ICommand;
};

namespace Utility{
	class Options;
	class EventLogRecord;
};
//WinApi
typedef struct _EVENTLOGRECORD EVENTLOGRECORD;

namespace std{
	typedef __int64 __time64_t;
	typedef __time64_t time_t;

	template<class _Elem> struct char_traits;
	template<class _Ty> class allocator;
	template<class _Ty, class _Alloc = allocator<_Ty> > class vector;
	template<class _Ty1, class _Ty2> struct pair;
	template<class _Ty> struct less;
	template<class _Kty, class _Ty, class _Pr = less<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty> > > class map;
	template<class _Kty, class _Pr = less<_Kty>, class _Alloc = allocator<_Kty> > class set;
	template<class _Elem> struct char_traits;
	template<class _Elem, class _Traits, class _Alloc> class basic_string;
	template<class _Ty, class _Elem = char, class _Traits = char_traits<_Elem> > class ostream_iterator;

	typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;
};

namespace boost{
	namespace asio{
		template <typename Protocol> class stream_socket_service;
		template <typename Protocol, typename StreamSocketService = stream_socket_service< Protocol > > class basic_stream_socket;
		class io_service;
		template <typename Allocator> class basic_streambuf;
		typedef basic_streambuf<> streambuf;
		namespace ip{
			class tcp;
			template <typename InternetProtocol> class resolver_service;
			template <typename InternetProtocol> class basic_resolver_iterator;
			template <typename InternetProtocol, typename ResolverService = resolver_service< InternetProtocol > > class basic_resolver;
		};
	};
	namespace parameter { 
		struct void_; 
	};
	namespace lockfree {
		template <size_t Size> struct capacity;
		template <typename T, class A0 = boost::parameter::void_, class A1 = boost::parameter::void_>class spsc_queue;
	};
		
	namespace system{
		class error_code;
	};

	namespace program_options{
		class variables_map;
	};
	template<class T> class shared_ptr;
	class condition_variable;
	class mutex;
	//struct default_user_allocator_new_delete;
	//template <typename T, typename UserAllocator = default_user_allocator_new_delete> class object_pool;
};

#endif //__FWDHEADERS_H_INCLUDED__