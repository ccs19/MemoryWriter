#pragma once
#include "stdafx.h"
#include <vector>

namespace plexerCode {
	class ProcessHelper {
	public:
		static HANDLE getProcHandleByName(const char*);
		static HANDLE getProcHandleByPid(DWORD pid);
		static std::vector<DWORD>* getAllProcHandles();
		static DWORD getLastError() { return lastError_; }

	private:
		static void setLastError() { lastError_ = GetLastError(); }
		static void cleanGetAllProcHandles(std::vector<DWORD>* pidsVector, DWORD* pidsArray, bool bail);

		static const unsigned long BUFF_SIZE = 256;
		static DWORD lastError_;
	};
};

