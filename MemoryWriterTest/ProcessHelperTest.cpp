      
#include "stdafx.h"
#include "../MemoryWriter/ProcessHelper.h"
using namespace plexerCode;
BOOST_AUTO_TEST_CASE( ProcessHelperTest )  
{
	auto pids = ProcessHelper::getAllProcPids();
	BOOST_CHECK(pids > 0);
}

