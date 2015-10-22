#ifndef __HTTP_CLIENT_H_INCLUDED__ 
#define __HTTP_CLIENT_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Http{

	class HttpClient {
	public:
		HttpClient( boost::asio::io_service&,
					const std::string&,
					const std::string&,
					const std::string&,
					const std::string& );

		virtual ~HttpClient(){}

	private:
		void handle_resolve( const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator );
		void handle_connect( const boost::system::error_code& );
		void handle_write_request( const boost::system::error_code& );
		void handle_read_status_line( const boost::system::error_code& );
		void handle_read_headers( const boost::system::error_code& );
		void handle_read_content( const boost::system::error_code& );

		boost::asio::ip::tcp::resolver resolver_;
		boost::asio::ip::tcp::socket socket_;
		boost::asio::streambuf request_;
		boost::asio::streambuf response_;
	};

};

#pragma pack( pop )

#endif //__HTTP_CLIENT_H_INCLUDED__
