#include "stdafx.h"
#include "ProcessHelper.h"
#include "plexerCodeUtil.h"
#include "ProcessConstants.h"

#define P_HELPER_SLASH  L"/\\"


namespace plexerCode {

	DWORD ProcessHelper::lastError_ = 0;

	HANDLE ProcessHelper::getProcHandleByName(const TCHAR* fileName) {
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
					setLastError();
					LOG(DEBUG) << "Failed to open process with PID " << *pid << ": " << ProcessConstants::errorToString(getLastError());
				}
				if(found) {
					break;
				}
			}
		}
		return result;
	}

	bool ProcessHelper::compareFileNames(HANDLE* result,const TCHAR* fileName) {
		TCHAR name[MAX_PATH];
		auto valid = 0;
		auto getNameSuccess = GetProcessImageFileName(result, name, MAX_PATH);
		if (getNameSuccess > 0) {
			String path(name);
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


	bool ProcessHelper::reallocPidsBuffer(unsigned long& buffSize, std::vector<unsigned long>* pidsVector, DWORD*& pidsArray, DWORD bytesReturned) {
		LOG(DEBUG) << "Allocating more memory for pid buffer. Tried " << buffSize << ". Will try " << buffSize * 2;
		cleanGetAllProcPids(nullptr, pidsArray, false);
		buffSize *= 2;
		pidsArray = initDWord(buffSize);
		if (!EnumProcesses(pidsArray, buffSize, &bytesReturned)) {
			cleanGetAllProcPids(pidsVector, pidsArray, true);
			return true;
		}
		return false;
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
			LOG(ERROR) << "Process enumeration failed.";
		}
		else {
			auto loopFinished = false;
			while (!loopFinished) {
				if ((bytesReturned/sizeof DWORD) >= buffSize) {
					loopFinished = reallocPidsBuffer(buffSize, pidsVector, pidsArray, bytesReturned);
				}
				else {
					for (auto i = 0; i < (bytesReturned / sizeof DWORD); i++) {
						pidsVector->push_back(pidsArray[i]);
					}
					loopFinished = true;
				}
			}
		}
		cleanGetAllProcPids(nullptr, pidsArray, false);
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

	std::string getProcessName(HANDLE processHandle) {
		LPTSTR str;
		return nullptr;
	}

};