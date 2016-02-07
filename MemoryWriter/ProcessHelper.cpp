#include "stdafx.h"
#include "ProcessHelper.h"
#include "plexerCodeUtil.h"
#include "ProcessConstants.h"

#ifndef UNICODE  
#define P_HELPER_SLASH  "/\\"
#else
#define P_HELPER_SLASH  L"/\\"
#endif

namespace plexerCode {

	DWORD ProcessHelper::lastError_ = 0;

	HANDLE ProcessHelper::getProcHandleByName(TCHAR* fileName) {
		LOG(DEBUG) << "Searching for process handle for " << fileName;
		auto pids = getAllProcPids();
		auto found = false;
		HANDLE result = nullptr;
		if (pids) {
			for (auto pid = (*pids).begin(); pid != (*pids).end(); ++pid) {
				result = OpenProcess(PROCESS_QUERY_INFORMATION, true, *pid);
				if(result != nullptr) {
					found = compareFileNames(&result, fileName);
				}else {
					LOG(DEBUG) << "Failed to open process " << GetLastError();
				}
				if(found) {
					break;
				}
			}
		}
		return result;
	}

	bool ProcessHelper::compareFileNames(HANDLE* result, TCHAR* fileName) {
		TCHAR name[MAX_PATH];
		auto valid = false;
		auto getNameSuccess = GetProcessImageFileName(result, name, MAX_PATH);
		if (getNameSuccess > 0) {
			ProcString path(name);
			auto fileNameIndex = path.find_last_of(P_HELPER_SLASH);
			if(fileNameIndex >= 0) {
				auto fName = path.substr(fileNameIndex + 1);
				valid = fName.compare(fileName);
				LOG(DEBUG) << "Process name: " << fName;
			}
		} 
		else {
			setLastError();
		}
		if(!valid && result != nullptr && getNameSuccess) {
			CloseHandle(result);
		}
		return valid;
	}



	/**
	  * Returns an allocated vector containing all running process pids or
	  * nullptr if failed. Call getLastError to see failure cause.
	 **/
	std::vector<DWORD>* ProcessHelper::getAllProcPids() {
		auto buffSize = BUFF_SIZE;
		auto pidsVector = new std::vector<DWORD>();
		auto pidsArray = initDWord(buffSize);
		DWORD bytesReturned = 0;
		auto result = EnumProcesses(pidsArray, buffSize, &bytesReturned);
		if (!result) {
			cleanGetAllProcPids(pidsVector, pidsArray, true);
		}
		else {
			while (true) {
				if ((bytesReturned/sizeof DWORD) >= buffSize) {
					LOG(DEBUG) << "Allocating more memory for pid buffer. Tried " << buffSize << ". Will try " << buffSize * 2;
					cleanGetAllProcPids(nullptr, pidsArray, false);
					buffSize *= 2;
					pidsArray = initDWord(buffSize);
					if (!EnumProcesses(pidsArray, buffSize, &bytesReturned)) {
						cleanGetAllProcPids(pidsVector, pidsArray, true);
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
		cleanGetAllProcPids(nullptr, pidsArray, false);
		pidsVector->shrink_to_fit();
		return pidsVector;
	}

	void ProcessHelper::cleanGetAllProcPids(std::vector<DWORD>* pidsVector, DWORD* pidsArray, bool bail) {
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