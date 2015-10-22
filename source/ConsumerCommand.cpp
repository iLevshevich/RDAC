#include "stdafx.h"
#include "Singleton.h"
#include "Options.h"
#include "EventLogJournals.h"
#include "EventLogRecord.h"
#include "HttpClient.h"
#include "ICommand.h"
#include "Singleton.h"
#include "Utility.h"
#include "ConsumerCommand.h"

void Command::ConsumerCommand::execute(){
	try{
		Utility::EventLogJournals& journals( Memmory::Singleton< Utility::EventLogJournals >::Instance() );
		const std::string level( _bstr_t( log.getEventTypeName().c_str() ) );
		const std::string code( _bstr_t( log.getEventID().c_str() ) );
		if( journals.isExist( journal, level, code ) &&  
			__MODULE_NAME__ != log.getSourceName() ){ 

			std::wostringstream ss;
			{
				ss << L"{";
				ss << L"\"" << L"EventLogRecordNumber" << L"\":\"" << log.getRecordNumber() <<  L"\",";
				ss << L"\"" << L"EventLogTimeGenerated" << L"\":\"" << log.getTimeGenerated() <<  L"\",";
				ss << L"\"" << L"EventLogTimeWritten" << L"\":\"" << log.getTimeWritten() <<  L"\",";
				ss << L"\"" << L"EventLogId" << L"\":\"" << log.getEventID() <<   L"\",";
				ss << L"\"" << L"EventLogType" << L"\":\"" << log.getEventTypeName() <<  L"\",";
				ss << L"\"" << L"EventLogCategory" << L"\":\"" << log.getEventCategory() <<  L"\",";
				ss << L"\"" << L"EventLogRecordMessage" << L"\":\"";
				{
					std::vector< std::wstring >strings( log.getStrings() );
					std::for_each( strings.begin(), strings.end(), []( std::wstring& val_ ){	
						 std::replace_if( val_.begin(), val_.end(), []( std::wstring::value_type char_ ){ return char_== L'\b' || char_== L'\f' || char_== L'\n' || char_== L'\r' || char_== L'\t'; }, L' ' ); 
						 std::replace_if( val_.begin(), val_.end(), []( std::wstring::value_type char_ ){ return char_== L'\"'; }, L'\'' ); 

						 std::wstring result;
						 std::transform( val_.begin(), val_.end(), val_.begin(), [&result]( std::wstring::value_type char_ )->std::wstring::value_type{ if( char_== '\\' ){ result.append( L"\\\\\\\\" ); }else{ result.append( &char_ ); } return char_; });
						 std::swap( val_, result );
					} );

					std::copy(strings.begin(), strings.end(), std::ostream_iterator< std::wstring, wchar_t >(ss, L" "));
				}
				ss << L"\",";
				ss << L"\"" << L"EventLogSource" << L"\":\"" << log.getSourceName() <<  L"\",";
				ss << L"\"" << L"EventLogMachine" << L"\":\"" << log.getComputername() <<  L"\",";
					const std::wstring journal_( _bstr_t( journal.c_str() ) );
				ss << L"\"" << L"EventLogJournal" << L"\":\"" << journal_ <<  L"\",";
				ss << L"\"" << L"GUID" << L"\":\"" << "3170AA1C-2A18-4396-B2EF-671773929AF3" <<  L"\"";
				ss << L"}";
			}
			const std::string request( _bstr_t( ss.str().c_str() ) );

			Utility::Options& options( Memmory::Singleton< Utility::Options >::Instance() );
			const std::string server_host( options.get( "server_host" ) );
			const std::string server_port( options.get( "server_port" ) );
			const std::string customer_path( options.get( "customer_path" ) );

			boost::asio::io_service io_service;
			Http::HttpClient client(io_service, server_host, server_port, customer_path, request);
			io_service.run();
#if defined(__TRACERT__)
			{
				Utility::Options& options( Memmory::Singleton< Utility::Options >::Instance() );
				const _bstr_t server_host( options.get( "server_host" ).c_str() );
				const _bstr_t server_port( options.get( "server_port" ).c_str() );
				const _bstr_t customer_path( options.get( "customer_path" ).c_str() );
				const std::wstring request_( _bstr_t( ss.str().c_str() ) );
				Utility::to_info_event_log( std::wstring( server_host ) + L":" + std::wstring( server_port ) + std::wstring( customer_path ) + request_ );
			}
#endif
		}
#if defined(__TRACERT__)
		/*if( __MODULE_NAME__ != log.getSourceName() ){
			const std::wstring level_( _bstr_t( log.getEventTypeName().c_str() ) );
			const std::wstring code_( _bstr_t( log.getEventID().c_str() ) );
			const std::wstring journal_( _bstr_t( journal.c_str() ) );
			Utility::to_info_event_log( journal_ + L" " + level_ + L" " + code_ );
		}*/
#endif
		setSuccess();
	}catch( const std::runtime_error& re ){
		setError();
		const std::wstring error_message( _bstr_t( re.what() ) );
		Utility::to_error_event_log( error_message );
	}catch(...){
		setError();
		Utility::to_error_event_log( L"Unknown Error!" );
	}
}

Command::ConsumerCommand::ConsumerCommand( const std::string& journal_,
										   const Command::ConsumerCommand::BaseType& log_ ):
			journal( journal_ ),
			log( log_ ),
			state( success ){}

Command::ConsumerCommand::~ConsumerCommand(){}

bool Command::ConsumerCommand::isSuccess() const{
	return state == success;
}

bool Command::ConsumerCommand::isError() const{
	return state == error;
}

void Command::ConsumerCommand::setSuccess(){
	state = success;
}

void Command::ConsumerCommand::setError(){
	state = error;
}