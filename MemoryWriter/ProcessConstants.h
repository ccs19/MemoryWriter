#pragma once
#include <string>

#ifndef UNICODE  
typedef std::string ProcString;
#else
typedef std::wstring ProcString;
#endif



namespace plexerCode {
	struct ProcessErrorCodes {
		int code;
		char* msg;
	};
	class ProcessConstants {

	private:
		//ProcessErrorCodes* ERR_5(5,"ACCESS DENIED");
	};
};
