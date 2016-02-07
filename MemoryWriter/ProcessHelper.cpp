#include "stdafx.h"
#include "ProcessHelper.h"
#include "plexerCodeUtil.h"

namespace plexerCode {
	DWORD ProcessHelper::lastError_ = 0;

	HANDLE ProcessHelper::getProcHandleByName(const char*) {
		return nullptr;
	}

	HANDLE ProcessHelper::getProcHandleByPid(DWORD pid) {
		return nullptr;
	}

	/**
	  * Returns an allocated vector containing all running process pids or
	  * nullptr if failed. Call getLastError to see failure cause.
	 **/
	std::vector<DWORD>* ProcessHelper::getAllProcHandles() {
		auto buffSize = BUFF_SIZE;
		auto pidsVector = new std::vector<DWORD>();
		auto pidsArray = initDWord(buffSize);
		DWORD bytesReturned = 0;
		auto result = EnumProcesses(pidsArray, buffSize, &bytesReturned);
		if (!result) {
			cleanGetAllProcHandles(pidsVector, pidsArray, true);
		}
		else {
			while (true) {
				if (bytesReturned >= buffSize) {
					cleanGetAllProcHandles(nullptr, pidsArray, false);
					buffSize *= 2;
					pidsArray = initDWord(buffSize);
					if (!EnumProcesses(pidsArray, buffSize, &bytesReturned)) {
						cleanGetAllProcHandles(pidsVector, pidsArray, true);
						break;
					}
				}
				else {
					for (auto i = 0; i < (bytesReturned / sizeof DWORD); i++) {
						pidsVector->push_back(pidsArray[i]);
					}
					break;
				}
			}
		}
		cleanGetAllProcHandles(nullptr, pidsArray, false);
		pidsVector->shrink_to_fit();
		return pidsVector;
	}

	void ProcessHelper::cleanGetAllProcHandles(std::vector<DWORD>* pidsVector, DWORD* pidsArray, bool bail) {
		if (pidsVector != nullptr) {
			delete(pidsVector);
			pidsVector = nullptr;
		}
		if (pidsArray != nullptr) {
			delete(pidsArray);
			pidsArray = nullptr;
		}
		if (bail) {
			setLastError();
		}
	}

};