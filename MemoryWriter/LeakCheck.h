#pragma once
#if _DEBUG
#define _CRTDBG_MAP_ALLOC

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
					if(result == 0) {
						std::wstringstream wss;
						wss << "LEAK CHECK: No memory leaks detected" << std::endl;
						OutputDebugString(wss.str().c_str());
					}
				}
				void static leakChk_() {
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