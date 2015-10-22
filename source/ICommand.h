#ifndef __I_COMMAND_H_INCLUDED__ 
#define __I_COMMAND_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "fwdheaders.h"

#pragma pack( push, 8 )

namespace Command{

	class ICommand{
	public:
		virtual void execute() = 0;

		virtual bool isSuccess() const = 0;
		virtual bool isError() const = 0;

		virtual ~ICommand(){}
	};

};

#pragma pack( pop )

#endif //__I_COMMAND_H_INCLUDED__