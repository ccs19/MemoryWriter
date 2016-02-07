#include "stdafx.h"
#include "ProcessHelper.h"

HANDLE plexerCode::ProcessHelper::getProcHandleByName(const char*) {
	return nullptr;
}

HANDLE plexerCode::ProcessHelper::getProcHandleByPid(DWORD pid) {
	return nullptr;
}

/**
  * Returns an allocated vector containing all running process handles or 
  * nullptr if failed. Call getLastError to see failure cause.
 **/
std::vector<HANDLE> plexerCode::ProcessHelper::getAllProcHandles() {
	auto buffSize = BUFF_SIZE;
	auto handles = new std::vector<HANDLE>();
	auto* processIds = new DWORD[buffSize];
	DWORD bytesReturned;

	auto result = EnumProcesses(processIds, buffSize, &bytesReturned);
	if(!result) {
		delete(handles);
		handles = nullptr;
		lastError_ = GetLastError();
	}else {
		
	}

	delete(processIds);
	return handles;
}


