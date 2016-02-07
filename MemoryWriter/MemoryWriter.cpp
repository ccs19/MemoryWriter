// MemoryWriter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Process.h"


int main()
{
	plexerCode::Process process;
	if(!process.attachProcess("HelloWorldLoop")) {
		//std::cout << process.getFriendlyErrMsg() << std::endl;
	}else {
		std::cout << "SUCCESS!";
	}
	getchar();
}

