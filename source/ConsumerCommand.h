#ifndef __CONSUMER_COMMAND_H_INCLUDED__ 
#define __CONSUMER_COMMAND_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Command{

	class ConsumerCommand: public ICommand{
	public:
		typedef Utility::EventLogRecord BaseType;

		ConsumerCommand( const std::string&,
						 const BaseType& );

		void execute();

		bool isSuccess() const;
		bool isError() const;

		virtual ~ConsumerCommand();

	private:
		void setSuccess();
		void setError();

		BaseType log;
		std::string journal;

		enum {max_length = 1024};
		enum { success, error } state;
	};

};

#pragma pack( pop )

#endif //__CONSUMER_COMMAND_H_INCLUDED__