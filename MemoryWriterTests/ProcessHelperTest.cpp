
#include "stdafx.h"
#include "TestCase.h"
#include "Process.h"
#include "ProcessConstants.h"
#include "ProcessHelper.h"

using namespace plexerCode;

SCENARIO("Process Helper returns valid Process objects", "[ProcessHelper]") {
	
	GIVEN("The HelloWorlProcess") {
		std::wstring procName = L"HelloWorldLoop";
		plexerCode::Process* proc;

		WHEN("Get process by name") {
			proc = ProcessHelper::getProcessByName(procName.c_str());

				THEN("Handle should not be null") {
					REQUIRE(proc != nullptr);
				}
		}
	}
}