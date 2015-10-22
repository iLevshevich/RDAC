#ifndef __EVENT_LOG_RECORD_H_INCLUDED__ 
#define __EVENT_LOG_RECORD_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Utility{

	class EventLogRecord{
	public:
		typedef EVENTLOGRECORD BaseType;
		typedef EVENTLOGRECORD* BasePointerType;
		

		EventLogRecord( BasePointerType const );

		virtual ~EventLogRecord();

		std::wstring getRecordNumber() const; 
		std::wstring getTimeGenerated();
		std::wstring getTimeWritten();  
		std::wstring getEventID() const;
		std::wstring getEventType() const;
		std::wstring getEventTypeName() const;
		std::wstring getEventCategory() const;
		std::vector< std::wstring > getStrings() const;
		std::wstring getSourceName() const;
		std::wstring getComputername() const;

		bool isEventError() const;
		bool isEventWarning() const;
		bool isEventInformation() const;
		bool isEventAuditSuccess() const;
		bool isEventAuditFailure() const;

	private:
		std::wstring __fastcall getDateTime( std::time_t* const );

		boost::shared_ptr< BaseType > event_log_record;
	};

};

#pragma pack( pop )

#endif //__EVENT_LOG_RECORD_H_INCLUDED__