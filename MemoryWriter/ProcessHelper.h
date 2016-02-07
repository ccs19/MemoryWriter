#pragma once
#include "stdafx.h"
#include <vector>

namespace plexerCode {
	class ProcessHelper {
	public:
		static HANDLE getProcHandleByName(TCHAR* fileName);
		static std::vector<DWORD>* getAllProcPids();
		static DWORD getLastError() { return lastError_; }

	private:
		static bool compareFileNames(HANDLE* result, TCHAR* fileName);
		static void setLastError() { lastError_ = GetLastError(); }
		static void cleanGetAllProcPids(std::vector<DWORD>* pidsVector, DWORD* pidsArray, bool bail);
		static const unsigned long BUFF_SIZE = 256;
		static DWORD lastError_;
	};
};

