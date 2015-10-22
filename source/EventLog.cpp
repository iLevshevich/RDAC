#include "stdafx.h"
#include "Options.h"
#include "EventLogJournals.h"
#include "Singleton.h"
#include "Buffer.h"
#include "EventLogRecord.h"
#include "ICommand.h"
#include "ConsumerCommand.h"
#include "Utility.h"
#include "EventLog.h"

EntryPoint::EventLog::EventLog( const std::string& journal_ ):
	consumer_( boost::thread( boost::bind( &EntryPoint::EventLog::consumer, this ) ) ),
	producer_( boost::thread( boost::bind( &EntryPoint::EventLog::producer, this, journal_ ) ) ),
	error_( boost::thread( boost::bind( &EntryPoint::EventLog::error, this ) ) ){
		mutex.lock();
		mutex_error.lock();
}

EntryPoint::EventLog::~EventLog(){
	if( producer_.joinable() ){
		producer_.interrupt();
		producer_.timed_join( boost::posix_time::seconds( timejoin ) );
		//consumer_.join();
	}

	if( consumer_.joinable() ){
		consumer_.interrupt();
		consumer_.timed_join( boost::posix_time::seconds( timejoin ) );
		//consumer_.join();
	}

	if( error_.joinable() ){
		error_.interrupt();
		error_.timed_join( boost::posix_time::seconds( timejoin ) );
		//error_.join();
	}
}

void EntryPoint::EventLog::error(){
	try{
		Utility::Options& options( Memmory::Singleton< Utility::Options >::Instance() );
		const std::size_t attempts_number( boost::lexical_cast< std::size_t >( options.get( "attempts_number" ) ) );
		const std::size_t sleep_time( boost::lexical_cast< std::size_t >( options.get( "sleep_time" ) ) ); //in seconds

		while ( true ) {
			boost::unique_lock<boost::mutex> lock( mutex_error );
			{
				boost::this_thread::interruption_point(); 

				boost::shared_ptr< Command::ICommand > command;
				if( spsc_queue_error.pop( command ) ){

					for( std::size_t iter( 0 ); iter < attempts_number; ++iter ){
						boost::this_thread::disable_interruption di;
						command->execute();

						if( command->isSuccess() ){
							break;
						}else if( command->isError() ){
							boost::this_thread::restore_interruption ri( di );
							boost::xtime time;
							{
								boost::xtime_get( &time, boost::TIME_UTC_ );
								time.sec += sleep_time;
							}
							boost::this_thread::sleep( time );
						}
					}

				}else{
					condition_error.wait( lock );
				}
			}
		}
	}catch( const boost::thread_interrupted& ){
		//std::cerr << "Interrupted: " << boost::this_thread::get_id() << std::endl;
	}catch( const std::runtime_error& re ){
		const std::wstring error_message( _bstr_t( re.what() ) );
		Utility::to_error_event_log( error_message );
	}catch(...){
		Utility::to_error_event_log( L"Unknown Error!" );
	}
}

void EntryPoint::EventLog::consumer(){
	try{
		while ( true ) {
			boost::unique_lock<boost::mutex> lock( mutex );
			{
				boost::this_thread::interruption_point(); 

				boost::shared_ptr< Command::ICommand > command;
				if( spsc_queue.pop( command ) ){

					{
						boost::this_thread::disable_interruption di;
						command->execute();

						if( command->isError() ){
							spsc_queue_error.push( command );
							condition_error.notify_one();
						}
					}

				}else{
					condition.wait( lock );
				}
			}
		}
	}catch( const boost::thread_interrupted& ){
		//std::cerr << "Interrupted: " << boost::this_thread::get_id() << std::endl;
	}catch( const std::runtime_error& re ){
		const std::wstring error_message( _bstr_t( re.what() ) );
		Utility::to_error_event_log( error_message );
	}catch(...){
		Utility::to_error_event_log( L"Unknown Error!" );
	}
}

void EntryPoint::EventLog::producer( const std::string& log_name_ ){
	try{
		boost::shared_ptr< void > event_log( CreateEvent( NULL, TRUE, FALSE, NULL ), []( void* const ptr ){ CloseHandle(ptr); } );
		if ( !event_log.get() ) {
			throw std::runtime_error( ( boost::format( "CreateEvent failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
		}

		const _bstr_t log_name( log_name_.c_str() );
		boost::shared_ptr< void > log( OpenEventLog( NULL, log_name ), []( void* const ptr ){ CloseEventLog(ptr); } );
		if ( !log.get() ) {
			throw std::runtime_error( ( boost::format( "OpenEventLog failed with with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
		}

		if ( !NotifyChangeEventLog( log.get(), event_log.get() ) ) {
			throw std::runtime_error( ( boost::format( "NotifyChangeEventLog failed with with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
		}

		unsigned long record_skip( 0 );
		if( !GetNumberOfEventLogRecords( log.get(), &record_skip ) ) {
			throw std::runtime_error( ( boost::format( "GetOldestEventLogRecord failed with with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
		}

		unsigned long long record_iter( 0 );
		while( true ) {
			const unsigned long wait_reason( ::WaitForSingleObject( event_log.get(), timeout ) );
			boost::this_thread::interruption_point();
			
			{
				boost::this_thread::disable_interruption di;

				if ( WAIT_TIMEOUT == wait_reason ) {
					continue;
				}else if ( WAIT_OBJECT_0 == wait_reason ) {
					Memmory::Buffer< unsigned char > buffer( 0 );

					unsigned long status = ERROR_SUCCESS;
					while ( ERROR_SUCCESS == status )
					{
						unsigned long bytes_read( 0 );
						unsigned long minimum_bytes_to_read( 0 );

						if ( !ReadEventLog(	log.get(), 
											EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ,
	 									    NULL, 
											buffer.get(),
											buffer.size(),
											&bytes_read,
											&minimum_bytes_to_read ) ) {
							status = GetLastError();
							if ( ERROR_INSUFFICIENT_BUFFER == status ) {
								status = ERROR_SUCCESS;
								const std::size_t buffer_size( minimum_bytes_to_read );
								buffer.set( new Memmory::Buffer< unsigned char >::BaseType[ buffer_size ], buffer_size );
								::memset( buffer.get(), 0, buffer_size );
							}else if( ERROR_EVENTLOG_FILE_CHANGED == status ){ //reinit

									{
										record_skip = 0;
										record_iter = 0;
									}

									event_log.reset( CreateEvent( NULL, TRUE, FALSE, NULL ), []( void* const ptr ){ CloseHandle(ptr); } );
									if ( !event_log.get() ) {
										throw std::runtime_error( ( boost::format( "CreateEvent failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
									}

									log.reset( OpenEventLog( NULL, log_name ), []( void* const ptr ){ CloseEventLog(ptr); } );
									if ( !log.get() ) {
										throw std::runtime_error( ( boost::format( "OpenEventLog failed with with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
									}

									if ( !NotifyChangeEventLog( log.get(), event_log.get() ) ) {
										throw std::runtime_error( ( boost::format( "NotifyChangeEventLog failed with with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
									}

									Utility::to_warn_event_log( L"The event log file has changed between read operations" );

									status = ERROR_SUCCESS;
									continue;
							} else {
								if (ERROR_HANDLE_EOF != status) {
									throw std::runtime_error( ( boost::format( "ReadEventLog failed with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
								}
							}
						} else {
							 EVENTLOGRECORD* record( reinterpret_cast< EVENTLOGRECORD* >( buffer.get() ) );
							 while( bytes_read > 0 ) {

								 if( ++record_iter > record_skip ){
									spsc_queue.push( boost::shared_ptr< Command::ICommand >( new Command::ConsumerCommand( log_name_, Utility::EventLogRecord( record ) ) ) );
									condition.notify_one();
								 }

								 bytes_read -= record->Length;     
								 record = reinterpret_cast< EVENTLOGRECORD* >( reinterpret_cast< unsigned char* >( record ) + record->Length );
							 }

							 unsigned long record_length( 0 );
							 if( !GetNumberOfEventLogRecords( log.get(), &record_length ) ) {
								 throw std::runtime_error( ( boost::format( "GetOldestEventLogRecord failed with with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
							 }

							 if( record_iter >= record_length ){
								break;
							 }
						}
					}

					ResetEvent( event_log.get() );
				} else {
					if ( WAIT_FAILED == wait_reason ) {
						throw std::runtime_error( ( boost::format( "WaitForSingleObject failed with with %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
					}
					break;
				}
			}
		}
	}catch( const boost::thread_interrupted& ){
		//std::cerr << "Interrupted: " << boost::this_thread::get_id() << std::endl;
	}catch( const std::runtime_error& re ){
		const std::wstring error_message( _bstr_t( re.what() ) );
		Utility::to_error_event_log( error_message );
	}catch(...){
		Utility::to_error_event_log( L"Unknown Error!" );
	}
}

void EntryPoint::EventLog::run(){
	mutex.unlock();
	mutex_error.unlock();
}
		
void EntryPoint::EventLog::stop(){
	mutex.lock();	
	mutex_error.lock();
}