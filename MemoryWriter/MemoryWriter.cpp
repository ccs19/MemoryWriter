// MemoryWriter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Process.h"
#include "ProcessHelper.h"
#include "plexerCodeUtil.h"

MEM_LEAK_CHECK

INITIALIZE_EASYLOGGINGPP

using namespace plexerCode;
int main()
{
	el::Configurations conf("logger.cfg");
	el::Loggers::reconfigureAllLoggers(conf);

	auto pids = plexerCode::ProcessHelper::getAllProcPids();
	auto result = ProcessHelper::getProcessByName(L"HelloWorldLoop");
	if(result!= nullptr) {
		LOG(DEBUG) << "FOUND!";
	}
	auto procName = ProcessHelper::getProcessName(result);
	if(procName.length() > 0){
		LOG(DEBUG) << "Proc name: " << procName;
	}else {
		LOG(DEBUG) << "Failed: " << ProcessConstants::errorToString(ProcessHelper::getLastError());	
	}
	LOG(DEBUG) << "Vector len:" << pids->size();
	
	if (result) {
		Process proc(result);
		proc.killProcess();
		proc.killProcess();
	}

	getchar();
}


