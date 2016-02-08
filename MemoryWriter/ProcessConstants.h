#pragma once
#include <string>

#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif


namespace plexerCode {
	
	class ProcessConstants {
		enum Error {
			access_denied = 5, invalid_parameter = 87,
			invalid_handle = 6, partial_copy = 299
	};

	public:

		static String errorToString(DWORD errorNum) {
			String result;
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


	private:
	};
};
