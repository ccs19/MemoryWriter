#pragma once
#include "StringDefines.h"

namespace plexerCode {
	
	class ProcessConstants {
	public:

/**
Defines:
PID = PC_PID
Process handle = PC_HANDLE

*/

#if defined(_MSC_VER) //Windows specific definitions
#define PC_PID DWORD
#define PC_HANDLE HANDLE
#define PC_RESULT_CODE DWORD
#define PC_ACCESS_TYPE DWORD
#define PC_READ_ACCESS PROCESS_QUERY_INFORMATION | PROCESS_VM_READ 
#define PC_FULL_ACCESS PROCESS_ALL_ACCESS
#define PC_CHAR TCHAR
#endif //defined(MSC_VER)
#if defined(__linux) || defined(__linux__)

#endif//defined(__linux) || defined(__linux__)

		
//Unsupported 
#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__CYGWIN__)
#error "Compiler is not supported. MSVC is required under Windows."
#endif //defined(__MINGW32__) || defined(__MINGW64__)



		//Constants
		static const int WAIT_KILL_TIMEOUT = 5000;



		//Error info
		enum Error {
			access_denied = 5, invalid_parameter = 87,
			invalid_handle = 6, partial_copy = 299
		};

		enum WaitForSingleObjectCode {
			wfso_abandoned = WAIT_ABANDONED, wfso_success = WAIT_OBJECT_0, 
			wfso_timeout = WAIT_TIMEOUT, wfso_failed = WAIT_FAILED
		};





		static String errorToString(PC_RESULT_CODE errorNum) {
			std::wstringstream oss;
			switch(errorNum){
			case access_denied:
				oss << "Access Denied";
				break;
			case invalid_parameter:
				oss << "Invalid Parameter";
				break;
			case invalid_handle:
				oss << "Invalid Handle";
				break;
			case partial_copy:
				oss << "Partial Copy";
				break;
			default:
				oss << "Unknown error";
			}
			oss << " (" << errorNum << ")\0";
			return oss.str();
		}

		static String waitForSingleResultStr(PC_RESULT_CODE result) {
			std::wstringstream oss;
			switch(result) {
			case wfso_abandoned:
				oss << "Wait abandoned";
				break;
			case wfso_success:
				oss << "State signaled";
				break;
			case wfso_timeout:
				oss << "Timeout";
				break;
			case wfso_failed:
				oss << "Failure";
				break;
			}
			oss << " (" << result << ")\0";
			return oss.str();
		}

	private:
	};
};
