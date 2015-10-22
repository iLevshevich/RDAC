#include "stdafx.h"
#include "base64.h"
#include "Singleton.h"
#include "HttpClient.h"

Http::HttpClient::HttpClient( boost::asio::io_service& io_service,
							  const std::string& server, 
							  const std::string& port,
							  const std::string& path,
							  const std::string& data ):
									  resolver_( io_service ),
									  socket_( io_service ) {

		const std::string base64_data( Http::base64::base64_encode( (unsigned char const*)data.c_str() ) );
		std::ostream request_stream( &request_ );
		{
			request_stream << "POST " << path  << " HTTP/1.1\r\n";
			request_stream << "Host: " << server << "\r\n";
			request_stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; ru; rv:1.9.2.25) Gecko/20111212 Firefox/3.6.25 sputnik 2.5.1.2\r\n";
			request_stream << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
			request_stream << "Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3\r\n";
			//request_stream << "Accept-Encoding: gzip,deflate\r\n";
			request_stream << "Accept-Charset: windows-1251\r\n";
			request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
			request_stream << "Cache-Control: no-cache" << "\r\n";
			request_stream << "Connection: close\r\n";
			request_stream << "Content-Length: " << base64_data.length() << "\r\n\r\n";
			request_stream << base64_data;
		}

		boost::asio::ip::tcp::resolver::query query( server, port );
		resolver_.async_resolve( query,
			                     boost::bind( &Http::HttpClient::handle_resolve, 
											  this,
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::iterator ) );
}

void Http::HttpClient::handle_resolve( const boost::system::error_code& err,
                                       boost::asio::ip::tcp::resolver::iterator endpoint_iterator ) {
    if ( !err ) {
		boost::asio::async_connect( socket_, 
									endpoint_iterator,
									boost::bind( &Http::HttpClient::handle_connect, 
												 this,
												 boost::asio::placeholders::error ) );
    } else {
		throw boost::system::system_error( err );
    }
}

void Http::HttpClient::handle_connect( const boost::system::error_code& err ) {
    if ( !err ) {
		boost::asio::async_write( socket_, 
								  request_,
								  boost::bind( &Http::HttpClient::handle_write_request, 
											   this,
											   boost::asio::placeholders::error));
    } else {
		throw boost::system::system_error( err );
    }
}

void Http::HttpClient::handle_write_request( const boost::system::error_code& err ) {
    if ( !err ) {
		boost::asio::async_read_until(	socket_, 
										response_, 
										"\r\n",
										boost::bind( &Http::HttpClient::handle_read_status_line, 
													 this,
													 boost::asio::placeholders::error ) );
    } else {
		throw boost::system::system_error( err );
    }
}

void Http::HttpClient::handle_read_status_line( const boost::system::error_code& err ) {
    if ( !err )  {
		std::istream response_stream( &response_ );

		std::string http_version;
		response_stream >> http_version;

		unsigned int status_code;
		response_stream >> status_code;

		std::string status_message;
		std::getline(response_stream, status_message);

		if ( !response_stream || http_version.substr(0, 5) != "HTTP/" ) {
			throw std::runtime_error( "Invalid response" );
		}
		if ( status_code != 200 ) {
			throw std::runtime_error( ( boost::format( "Response returned with status code %d" ) % status_code ).str() );
		}

		boost::asio::async_read_until(	socket_, 
										response_, 
										"\r\n\r\n",
										boost::bind( &Http::HttpClient::handle_read_headers, 
													 this,
													 boost::asio::placeholders::error ) );
    } else {
		throw boost::system::system_error( err );
    }
}

void Http::HttpClient::handle_read_headers( const boost::system::error_code& err ) {
    if ( !err )  {

		{
			std::string response;
			response = std::string( std::istreambuf_iterator< char >( &response_ ), std::istreambuf_iterator< char >() );

			const std::string content_length_tag( "Content-Length:" );
			std::size_t content_length_val( 0 );
			{
				const std::string::size_type pos_begin = response.find( content_length_tag );
				if( pos_begin != std::string::npos ){
					const std::string::size_type pos_end = response.find( "\r\n", pos_begin );
					if( pos_end != std::string::npos ){
						content_length_val = boost::lexical_cast< std::size_t >( boost::trim_copy( response.substr( ( pos_begin + content_length_tag.length() ), ( pos_end - ( pos_begin + content_length_tag.length() ) ) ) ) );
					}
				}
			}
			
			const std::string data_tag( "\r\n\r\n" );
			std::string data_val;
			{
				const std::string::size_type pos_begin = response.find( data_tag );
				if( pos_begin != std::string::npos ){
					data_val = boost::trim_copy( response.substr( ( pos_begin + data_tag.length() ), content_length_val ) );
				}
			}

			if( "success" != data_val ){
				throw std::runtime_error( ( boost::format( "Response returned with %s" ) % data_val ).str() );
			}
 
		}

		boost::asio::async_read( socket_, 
								 response_,
								 boost::asio::transfer_at_least( 1 ),
								 boost::bind( &Http::HttpClient::handle_read_content, 
											  this,
											  boost::asio::placeholders::error ) );
    } else {
		throw boost::system::system_error( err );
    }
}

void Http::HttpClient::handle_read_content( const boost::system::error_code& err ) {
    if ( !err ) {

		boost::asio::async_read( socket_, 
								 response_,
								 boost::asio::transfer_at_least( 1 ),
								 boost::bind( &Http::HttpClient::handle_read_content, 
											  this,
											  boost::asio::placeholders::error ) );
    } else if ( err != boost::asio::error::eof ) {
		throw boost::system::system_error( err );
	} 
}