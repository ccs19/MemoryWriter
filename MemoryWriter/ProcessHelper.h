#pragma once
#include <vector>

namespace plexerCode {
	class ProcessHelper {
	public:
		static HANDLE getProcHandleByName(const char*);
		static HANDLE getProcHandleByPid(DWORD pid);
		static std::vector<HANDLE> getAllProcHandles();

	private:
		static const unsigned long BUFF_SIZE = 256;
		static DWORD lastError_;
	};
};
