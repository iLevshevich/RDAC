#include "stdafx.h"
#include "Utility.h"

void __fastcall Utility::to_error_event_log( const std::wstring& error_message ){
	LPWSTR message[1] = { const_cast< LPWSTR >( error_message.c_str() ) };
	const std::size_t count_strings( 1 );

	boost::shared_ptr< void > event_log( RegisterEventSource( NULL, __MODULE_NAME__ ), []( void* const ptr ){ if ( ptr ) DeregisterEventSource( ptr ); } );
	if ( !event_log.get() ){
		throw std::runtime_error( ( boost::format( "RegisterEventSource failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
	}
	
	if ( !ReportEvent( event_log.get(), 
					   EVENTLOG_ERROR_TYPE, 
					   NULL, 
					   NULL, 
					   NULL, 
					   count_strings, 
					   0, 
					   reinterpret_cast< LPCWSTR* >( const_cast< LPCWSTR* >( message ) ), 
					   NULL ) ){
		throw std::runtime_error( ( boost::format( "ReportEvent failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
	}
}

void __fastcall Utility::to_info_event_log( const std::wstring& error_message ){
	LPWSTR message[1] = { const_cast< LPWSTR >( error_message.c_str() ) };
	const std::size_t count_strings( 1 );

	boost::shared_ptr< void > event_log( RegisterEventSource( NULL, __MODULE_NAME__ ), []( void* const ptr ){ if ( ptr ) DeregisterEventSource( ptr ); } );
	if ( !event_log.get() ){
		throw std::runtime_error( ( boost::format( "RegisterEventSource failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
	}
	
	if ( !ReportEvent( event_log.get(), 
					   EVENTLOG_INFORMATION_TYPE, 
					   NULL, 
					   NULL, 
					   NULL, 
					   count_strings, 
					   0, 
					   reinterpret_cast< LPCWSTR* >( const_cast< LPCWSTR* >( message ) ), 
					   NULL ) ){
		throw std::runtime_error( ( boost::format( "ReportEvent failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
	}
}

void __fastcall Utility::to_warn_event_log( const std::wstring& error_message ){
	LPWSTR message[1] = { const_cast< LPWSTR >( error_message.c_str() ) };
	const std::size_t count_strings( 1 );

	boost::shared_ptr< void > event_log( RegisterEventSource( NULL, __MODULE_NAME__ ), []( void* const ptr ){ if ( ptr ) DeregisterEventSource( ptr ); } );
	if ( !event_log.get() ){
		throw std::runtime_error( ( boost::format( "RegisterEventSource failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
	}
	
	if ( !ReportEvent( event_log.get(), 
					   EVENTLOG_WARNING_TYPE, 
					   NULL, 
					   NULL, 
					   NULL, 
					   count_strings, 
					   0, 
					   reinterpret_cast< LPCWSTR* >( const_cast< LPCWSTR* >( message ) ), 
					   NULL ) ){
		throw std::runtime_error( ( boost::format( "ReportEvent failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
	}
}