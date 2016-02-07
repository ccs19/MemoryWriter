#pragma once

namespace plexerCode{};

namespace plexerCode {
	class Process {
	public:
		Process();
		~Process();
		bool detachProcess() const;
		bool isHandleOpen() const;
		bool attachProcess(std::string name);
		bool attachProcess(unsigned long pid);
		DWORD getLastError() const;
		std::string getFriendlyErrMsg() const;
	private:
		HANDLE procHandle_;
		unsigned long processNum_;
		bool lastOpSucceed_;
		DWORD lastError_;
		bool handleOpen_;
		void setLastError();
		void init();
	};
};
