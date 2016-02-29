#include "stdafx.h"

#include "Process.h"
#include "ProcessHelper.h"

plexerCode::Process::Process(PC_HANDLE handle) {
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

PC_RESULT_CODE plexerCode::Process::getLastError() const {
	return lastError_;
}

BOOL plexerCode::Process::killProcess() {
	PC_RESULT_CODE exitCode = 0;
	BOOL success = false;
	if(GetExitCodeProcess(procHandle_, &exitCode)) {
		if(exitCode == STILL_ACTIVE) {
			success = killAndWait();
		}else {
			LOG(DEBUG) << "Process not active.(Code: " << exitCode << ")";
		}
	}else {
		LOG(ERROR) << "GetExitCodeProcess failed.";
	}
	return success;
}

BOOL plexerCode::Process::killAndWait() {
	LOG(DEBUG) << "Process still alive. Terminating.";
	BOOL success = false;
	if (TerminateProcess(procHandle_, ERROR_SUCCESS)) {
		LOG(DEBUG) << "Sent exit signal to process. Waiting...";
		auto result = WaitForSingleObject(procHandle_, ProcessConstants::WAIT_KILL_TIMEOUT);
		LOG(DEBUG) << "WFSO result: " << ProcessConstants::waitForSingleResultStr(result);
	}
	else {
		setLastError();
		LOG(ERROR) << "Failed to terminate process. " << ProcessConstants::errorToString(getLastError());
	}
	return success;
}

String plexerCode::Process::getProcessName() {
	return ProcessHelper::getProcessName(procHandle_);
}

PC_PID plexerCode::Process::getProcessId() {
	return ProcessHelper::getProcessId(procHandle_);
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


