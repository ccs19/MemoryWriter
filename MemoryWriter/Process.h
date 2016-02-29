#pragma once
#include "ProcessConstants.h"

namespace plexerCode{};

namespace plexerCode {
	class Process {
	public:
		Process(PC_HANDLE handle);
		~Process();
		BOOL detachProcess() const;
		void setProcHandle(const PC_HANDLE procHandle) { procHandle_ = procHandle; }
		BOOL isHandleOpen() const;
		PC_RESULT_CODE getLastError() const;
		BOOL killProcess();
		BOOL killAndWait();
		String getProcessName();
		PC_PID getProcessId();
	private:

		PC_HANDLE procHandle_;
		unsigned long processNum_;
		BOOL lastOpSucceed_;
		PC_RESULT_CODE lastError_;
		BOOL handleOpen_;
		void setLastError();
		void init();
	};
};
