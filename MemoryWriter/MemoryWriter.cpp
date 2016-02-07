// MemoryWriter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Process.h"
#include "ProcessHelper.h"


INITIALIZE_EASYLOGGINGPP

int main()
{
	el::Configurations conf("logger.cfg");
	el::Loggers::reconfigureAllLoggers(conf);
	plexerCode::Process process;
	if(!process.attachProcess("HelloWorldLoop")) {
		//std::cout << process.getFriendlyErrMsg() << std::endl;
	}else {
		LOG(INFO) << "SUCCESS!";
	}
	auto pids = plexerCode::ProcessHelper::getAllProcHandles();
	for (auto i = (*pids).begin(); i != (*pids).end(); ++i) {
		LOG(DEBUG) << *i << std::endl;
	}
	getchar();
}

