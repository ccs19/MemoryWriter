#pragma once
#include <iostream>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

	namespace plexerCode{
		namespace leakCheck{
			class leakChk{
			public:
				leakChk() {
					leakChk_();
				}
			private:
				static void checkOnExit() {
					auto result = _CrtDumpMemoryLeaks();
				}
				void leakChk_() {
					std::atexit(checkOnExit);
				}
			};

		}
	}

#define MEM_LEAK_CHECK plexerCode::leakCheck::leakChk plexerCode_checkMemLeaks;

#endif

#ifndef MEM_LEAK_CHECK
#define MEM_LEAK_CHECK
#endif