#include "stdafx.h"

#include "Process.h"
plexerCode::Process::Process() {
	init();
}

plexerCode::Process::~Process() {
	CloseHandle(procHandle_);
}

bool plexerCode::Process::detachProcess() const {
	auto result = false;
	if(handleOpen_) {
		result = CloseHandle(procHandle_);
	}
	return result;
}

bool plexerCode::Process::isHandleOpen() const {
	return handleOpen_;
}

bool plexerCode::Process::attachProcess(std::string name) {
	procHandle_ = OpenProcess(PROCESS_ALL_ACCESS, true, 19352);
	if(procHandle_ == nullptr) {
		lastOpSucceed_ = false;
		handleOpen_ = true;
		setLastError();
	}else {
		lastOpSucceed_ = true;
	}
	return lastOpSucceed_;
}

bool plexerCode::Process::attachProcess(DWORD pid) {
	procHandle_ = OpenProcess(PROCESS_ALL_ACCESS, true, pid);
	if (procHandle_ == nullptr) {
		lastOpSucceed_ = false;
		setLastError();
	}
	else { //Successfully attached to process
		lastOpSucceed_ = true;
		handleOpen_ = true;
	}
	return lastOpSucceed_;
}

DWORD plexerCode::Process::getLastError() const {
	return lastError_;
}

std::string plexerCode::Process::getFriendlyErrMsg() const {
	std::string msg = "Last attempting operation on process failed. Reason: " + getLastError();
	return msg;
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
