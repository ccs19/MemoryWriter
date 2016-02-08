#include "stdafx.h"

#include "Process.h"
plexerCode::Process::Process(HANDLE handle) {
	procHandle_ = handle;
}

plexerCode::Process::~Process() {
	detachProcess();
}

BOOL plexerCode::Process::detachProcess() const {
	BOOL result = false;
	if(handleOpen_) {
		result = CloseHandle(procHandle_);
	}
	return result;
}

BOOL plexerCode::Process::isHandleOpen() const {
	return handleOpen_;
}

DWORD plexerCode::Process::getLastError() const {
	return lastError_;
}


void plexerCode::Process::setLastError() {
	lastError_ = GetLastError();
}

void plexerCode::Process::init() {
	lastError_ = 0;
	lastOpSucceed_ = true;
	procHandle_ = nullptr;
	processNum_ = 0;
	handleOpen_ = false;
}
