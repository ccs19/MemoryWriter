
#pragma once
#include "stdafx.h"
#include <vector>
#include "ProcessConstants.h"
#include "Process.h"

namespace plexerCode {
	//class Process;

	class DLLEXPORT ProcessHelper {
	public:
		static Process* getProcessByName(const PC_CHAR* fileName);
		static std::shared_ptr<std::vector<PC_PID>> getAllProcPids();
		static PC_RESULT_CODE getLastError() { return lastError_; }
		static String getProcessName(PC_HANDLE processHandle);
		static unsigned long getProcessId(PC_HANDLE procHandle);
	private:
		static bool compareFileNames(PC_HANDLE result,const PC_CHAR* fileName);
		static bool reallocPidsBuffer(unsigned long& buffSize, std::vector<PC_PID>* pidsVector, PC_PID*& pidsArray, PC_PID bytesReturned);
		static PC_HANDLE getProcessByPid(PC_ACCESS_TYPE desiredAccess, PC_PID pid);
		static void setLastError() { lastError_ = GetLastError(); }
		static void cleanGetAllProcPids(std::vector<PC_PID>* pidsVector, PC_PID* pidsArray, bool bail);
		static const unsigned long BUFF_SIZE = 1024;
		static PC_RESULT_CODE lastError_;
		static void closeHandle(PC_HANDLE handle);
	};
};

