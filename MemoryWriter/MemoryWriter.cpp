// MemoryWriter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Process.h"
#include "ProcessHelper.h"


INITIALIZE_EASYLOGGINGPP

int main()
{
	plexerCode::Process process;
	if(!process.attachProcess("HelloWorldLoop")) {
		//std::cout << process.getFriendlyErrMsg() << std::endl;
	}else {
		LOG(INFO) << "SUCCESS!";
	}
	auto pids = plexerCode::ProcessHelper::getAllProcHandles();
	for (auto i = (*pids).begin(); i != (*pids).end(); ++i) {
		std::cout << *i << std::endl;
	}
	getchar();
}

