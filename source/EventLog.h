#ifndef __EVENT_LOG_H_INCLUDED__ 
#define __EVENT_LOG_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace EntryPoint{

	class EventLog{
	public:
		EventLog( const std::string& );
		~EventLog();

		void run();
		void stop();

	private:
		void consumer();
		void producer( const std::string& );
		void error();

		enum { capacity_size = 4096 };
		boost::lockfree::spsc_queue< boost::shared_ptr< Command::ICommand >, boost::lockfree::capacity< capacity_size > > spsc_queue;
		boost::condition_variable condition;
		boost::mutex mutex;

		enum { capacity_size_error = 8192 };
		boost::lockfree::spsc_queue< boost::shared_ptr< Command::ICommand >, boost::lockfree::capacity< capacity_size_error > > spsc_queue_error;
		boost::condition_variable condition_error;
		boost::mutex mutex_error;

		enum { timeout = 10000 };
		enum { timejoin = 5 };
		boost::thread consumer_;
		boost::thread producer_;
		boost::thread error_;

	};

};

#pragma pack( pop )

#endif //__EVENT_LOG_H_INCLUDED__


