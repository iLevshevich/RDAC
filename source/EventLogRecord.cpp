#include "stdafx.h"
#include "EventLogRecord.h"

Utility::EventLogRecord::EventLogRecord( Utility::EventLogRecord::BasePointerType const event_log_record_ ):
	event_log_record( reinterpret_cast< Utility::EventLogRecord::BasePointerType >( new char [event_log_record_->Length] ), []( void* const ptr ){
																																			if( ptr ){
																																				delete[] ptr; 
																																			}
																																		} ){
	memcpy( event_log_record.get(), event_log_record_, event_log_record_->Length );
}

Utility::EventLogRecord::~EventLogRecord(){}

std::wstring Utility::EventLogRecord::getRecordNumber() const{
	std::wostringstream ss;
	{
		ss << event_log_record->RecordNumber;
	}
	return ss.str();
}   

std::wstring Utility::EventLogRecord::getTimeGenerated(){
	const boost::posix_time::ptime current( boost::date_time::c_local_adjustor< boost::posix_time::ptime >::utc_to_local( boost::posix_time::from_time_t( static_cast< time_t >( event_log_record->TimeGenerated ) ) ) );

	std::wostringstream ss;
	{
#if !defined(__ISO_DATE_TIME_FORMAT__)
		ss.imbue( std::locale( std::locale( "Russian" ), new boost::posix_time::wtime_facet( L"%m.%d.%Y %H:%M:%S" ) ) );
		ss << current;
#else
		ss << boost::posix_time::to_iso_extended_wstring( current );
#endif
	}
	return ss.str();
}

std::wstring Utility::EventLogRecord::getTimeWritten(){
	const boost::posix_time::ptime current( boost::date_time::c_local_adjustor< boost::posix_time::ptime >::utc_to_local( boost::posix_time::from_time_t( static_cast< time_t >( event_log_record->TimeWritten ) ) ) );

	std::wostringstream ss;
	{
#if !defined(__ISO_DATE_TIME_FORMAT__)
		ss.imbue( std::locale( std::locale( "Russian" ), new boost::posix_time::wtime_facet( L"%m.%d.%Y %H:%M:%S" ) ) );
		ss << current;
#else
		ss << boost::posix_time::to_iso_extended_wstring( current );
#endif
	}
	return ss.str();
}
    
std::wstring Utility::EventLogRecord::getEventID() const{
	std::wostringstream ss;
	{
		ss << ( event_log_record->EventID & 0xFFFF );
	}
	return ss.str();
}
    
std::wstring Utility::EventLogRecord::getEventType() const{
	std::wostringstream ss;
	{
		ss << event_log_record->EventType;
	}
	return ss.str();
}

std::wstring Utility::EventLogRecord::getEventTypeName() const{
	      if( Utility::EventLogRecord::isEventError() ){
		return L"Error";	
	}else if( Utility::EventLogRecord::isEventWarning() ){
		return L"Warning";	
	}else if( Utility::EventLogRecord::isEventInformation() ){
		return L"Information";	
	}else if( Utility::EventLogRecord::isEventAuditSuccess() ){
		return L"AuditSuccess";	
	}else if( Utility::EventLogRecord::isEventAuditFailure() ){
		return L"AuditFailure";	
	}

	return L"";	
}
   
std::wstring Utility::EventLogRecord::getEventCategory() const{
	std::wostringstream ss;
	{
		ss << event_log_record->EventCategory;
	}
	return ss.str();
}
	
std::vector< std::wstring >Utility::EventLogRecord:: getStrings() const{
	std::vector< std::wstring > lines;
	const std::size_t value_type_size( sizeof( std::wstring::value_type ) );

	unsigned char* const current_record( reinterpret_cast< unsigned char* >( event_log_record.get() ) );
	unsigned long current_offset( reinterpret_cast< unsigned long >( event_log_record.get() ) + event_log_record->StringOffset );

	const std::size_t number_strings_read_length( event_log_record->NumStrings );
	for( std::size_t number_strings_read( 0 ); 
		             number_strings_read < number_strings_read_length; 
	               ++number_strings_read ){

		const std::wstring line( reinterpret_cast< std::wstring::const_pointer >( current_offset ) );
		{
			lines.push_back( line );
		}
		current_offset += ( line.length() * value_type_size/*weight of type*/ ) + value_type_size/*last null*/;
	}
    return lines;
}
    
std::wstring Utility::EventLogRecord::getSourceName() const{
	return reinterpret_cast< std::wstring::const_pointer >( reinterpret_cast< unsigned long >( event_log_record.get() ) + sizeof( EVENTLOGRECORD ) );
}
    
std::wstring Utility::EventLogRecord::getComputername() const{
	const std::size_t value_type_size( sizeof( std::wstring::value_type ) );
	return reinterpret_cast< const wchar_t* >( reinterpret_cast< unsigned long >( event_log_record.get() ) + 
											   sizeof( EVENTLOGRECORD ) + 
											 ( getSourceName().length() * value_type_size/*weight of type*/ ) + 
											   value_type_size/*last null*/ );
}

bool Utility::EventLogRecord::isEventError() const{
	return event_log_record->EventType == EVENTLOG_ERROR_TYPE;
}
	
bool Utility::EventLogRecord::isEventWarning() const{
	return event_log_record->EventType == EVENTLOG_WARNING_TYPE;
}
	
bool Utility::EventLogRecord::isEventInformation() const{
	return event_log_record->EventType == EVENTLOG_INFORMATION_TYPE;
}
	
bool Utility::EventLogRecord::isEventAuditSuccess() const{
	return event_log_record->EventType == EVENTLOG_AUDIT_SUCCESS;
}
	
bool Utility::EventLogRecord::isEventAuditFailure() const{
	return event_log_record->EventType == EVENTLOG_AUDIT_FAILURE;
}