#pragma once

namespace plexerCode{};

namespace plexerCode {
	class Process {
	public:
		Process(HANDLE handle);
		~Process();
		BOOL detachProcess() const;
		void setProcHandle(const HANDLE procHandle) { procHandle_ = procHandle; }
		BOOL isHandleOpen() const;
		DWORD getLastError() const;
		BOOL killProcess();
		BOOL killAndWait();
	private:
		HANDLE procHandle_;
		unsigned long processNum_;
		BOOL lastOpSucceed_;
		DWORD lastError_;
		BOOL handleOpen_;
		void setLastError();
		void init();
	};
};
