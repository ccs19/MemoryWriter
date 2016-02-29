#pragma once
#include "StringDefines.h"

namespace plexerCode {
	
	class ProcessConstants {
		enum Error {
			access_denied = 5, invalid_parameter = 87,
			invalid_handle = 6, partial_copy = 299
		};

		enum WaitForSingleObjectCode {
			wfso_abandoned = WAIT_ABANDONED, wfso_success = WAIT_OBJECT_0, 
			wfso_timeout = WAIT_TIMEOUT, wfso_failed = WAIT_FAILED
		};

	public:

		static String errorToString(DWORD errorNum) {
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

		static String waitForSingleResultStr(DWORD result) {
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

		static const int WAIT_KILL_TIMEOUT = 5000;

	private:
	};
};
