#pragma once

namespace plexerCode{};

namespace plexerCode {
	class Process {
	public:
		Process();
		~Process();
		BOOL detachProcess() const;
		BOOL isHandleOpen() const;
		BOOL attachProcess(std::string name);
		BOOL attachProcess(DWORD pid);
		DWORD getLastError() const;
		std::string getFriendlyErrMsg() const;
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
