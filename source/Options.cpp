#include "stdafx.h"
#include "Options.h"

void Utility::Options::init( int argc, _TCHAR* argv[] ){
	boost::program_options::options_description desc("Service options");

	desc.add_options()
			( "server_host", boost::program_options::value<std::string>()->default_value( "test-devsrv" ) )
			( "server_port", boost::program_options::value<std::string>()->default_value( "http" ) )
			( "customer_path", boost::program_options::value<std::string>()->default_value( "/EventLog/CreateByEventLog" ) )
			( "journals", boost::program_options::value<std::string>()->default_value( "Application(Error[All],Warning[All])|System(Error[All],Warning[All])|Security(AuditFailure[All])" ) )
			( "attempts_number", boost::program_options::value<std::string>()->default_value( "100" ) )
			( "sleep_time", boost::program_options::value<std::string>()->default_value( "60" ) ); // in seconds

    boost::program_options::store(boost::program_options::parse_command_line( argc, argv, desc ), vm);
}

std::string Utility::Options::get( const std::string& key ) const{
	return vm[ key ].as< std::string >();
}
