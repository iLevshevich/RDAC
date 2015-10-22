#include "stdafx.h"
#include "Singleton.h"
#include "Options.h"
#include "EventLogJournals.h"
#include "EventLog.h"
#include "Utility.h"

class CRDACModule : public CAtlServiceModuleT< CRDACModule, IDS_SERVICENAME >
{
public :
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_RDAC, "{01D73C58-D8F4-4A85-B27D-424443739977}")
	HRESULT InitializeSecurity() throw()
	{
		return S_OK;
	}

	HRESULT PreMessageLoop (int nShowCmd);
	HRESULT PostMessageLoop();
	void RunMessageLoop (void) throw();
	void OnContinue (void) throw();
	void OnPause (void) throw();
	void OnShutdown (void) throw();
	void OnStop (void) throw();
};

HRESULT CRDACModule::PreMessageLoop (int nShowCmd) throw()
{
	const HRESULT result( CAtlServiceModuleT< CRDACModule, IDS_SERVICENAME >::PreMessageLoop ( nShowCmd ) );
	if ( SUCCEEDED( result ) ){
		std::vector< boost::shared_ptr< EntryPoint::EventLog > >& logs( Memmory::Singleton< std::vector< boost::shared_ptr< EntryPoint::EventLog > > >::Instance() );
		for( auto& iter: logs ){
			iter->run();
		}
	}	
	return result;
}

HRESULT CRDACModule::PostMessageLoop()
{
	std::vector< boost::shared_ptr< EntryPoint::EventLog > >& logs( Memmory::Singleton< std::vector< boost::shared_ptr< EntryPoint::EventLog > > >::Instance() );
	for( auto& iter: logs ){
		iter->stop();
	}

    return CAtlServiceModuleT< CRDACModule, IDS_SERVICENAME >::PostMessageLoop(); 
}

void CRDACModule::RunMessageLoop () throw()
{
   	CAtlServiceModuleT< CRDACModule, IDS_SERVICENAME >::RunMessageLoop ();
}

void CRDACModule::OnContinue () throw()
{
    CAtlServiceModuleT< CRDACModule, IDS_SERVICENAME >::OnContinue ();
}

void CRDACModule::OnPause () throw()
{
    CAtlServiceModuleT< CRDACModule, IDS_SERVICENAME >::OnPause ();
}

void CRDACModule::OnShutdown () throw()
{
    OnStop ();
}

void CRDACModule::OnStop () throw()
{
    CAtlServiceModuleT< CRDACModule, IDS_SERVICENAME >::OnStop ();
}

CRDACModule _AtlModule;

extern "C" int WINAPI _tWinMain( HINSTANCE /*hInstance*/, 
								 HINSTANCE /*hPrevInstance*/, 
                                 LPTSTR /*lpCmdLine*/, 
								 int nShowCmd ){
	try{
		setlocale( LC_CTYPE, "windows-1251" );

		Utility::Options& options( Memmory::Singleton< Utility::Options >::Instance() );
		{
			int argc( 0 );
			const boost::shared_ptr< LPWSTR >argv( CommandLineToArgvW( GetCommandLine(), &argc ), []( void* const ptr ){ if( ptr ) LocalFree( ptr ); } );
			if ( !argv.get() ) {
				 throw std::runtime_error( ( boost::format( "Unable to parse command line %s" ) % boost::system::system_error( GetLastError(), boost::system::get_system_category() ).what() ).str() );
			}

			options.init( argc, argv.get() ); 
		}

		Utility::EventLogJournals& journals( Memmory::Singleton< Utility::EventLogJournals >::Instance() );
		{
			journals.init( options.get( "journals" ) );
		}

		std::vector< boost::shared_ptr< EntryPoint::EventLog > >& logs( Memmory::Singleton< std::vector< boost::shared_ptr< EntryPoint::EventLog > > >::Instance() );
		for( const auto& iter: journals.getJournals() ){
			boost::shared_ptr< EntryPoint::EventLog > obj ( new EntryPoint::EventLog( iter ) );
			logs.push_back( obj );
		}

		const int result( _AtlModule.WinMain( nShowCmd ) );
		if ( result != ERROR_SUCCESS ) {
			throw boost::system::system_error( GetLastError(), boost::system::get_system_category() );
		}
		return result;
	}catch( const std::runtime_error& re ){
		const std::wstring error_message( _bstr_t( re.what() ) );
		Utility::to_error_event_log( error_message );
	}catch(...){
		Utility::to_error_event_log( L"Unknown Error!" );
	}
	return 1;
}

