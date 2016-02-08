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
				result = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, true, *pid);
				if(result != nullptr) {
					found = compareFileNames(result, fileName);
				}else {
					setLastError();
					LOG(ERROR) << "Failed to open process with PID " << *pid << ": " << ProcessConstants::errorToString(getLastError());
				}
				if(found) {
					LOG(DEBUG) << "Found matching process for " << fileName << " with PID " << *pid;
					break;
				}
			}
		}
		if(!found) {
			LOG(DEBUG) << "Didn't find matching process for " << fileName;
			result = nullptr;
		}
		return result;
	}

	bool ProcessHelper::compareFileNames(HANDLE result,const TCHAR* fileName) {
		TCHAR name[MAX_PATH];
		auto valid = -1;
		auto getNameSuccess = GetProcessImageFileName(result, name, MAX_PATH);
		if (getNameSuccess > 0) {
			String path(name);
			String fName(L"");
			auto fileNameIndex = path.find_last_of(P_HELPER_SLASH);
			
			if(fileNameIndex >= 0) {
				fName.append(path.substr(fileNameIndex + 1));
				auto extensionIndex = fName.find_last_of(L"."); //Ignore file extension
				if (extensionIndex > 0) {
					fName = fName.substr(0, extensionIndex);
					valid = fName.compare(fileName);
				}
			}
			LOG(DEBUG) << "Process name: " << fName;
		} 
		else {
			setLastError();
			LOG(ERROR) << "Failed to get process name: " << ProcessConstants::errorToString(getLastError());
		}
		if(valid != 0 && result != nullptr) {
			CloseHandle(result);
		}
		return valid == 0 ? true : false;
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
			LOG(ERROR) << "Process enumeration failed: " << ProcessConstants::errorToString(getLastError());
		}
		else {
			auto loopFinished = false;
			while (!loopFinished) {
				if (bytesReturned >= buffSize * sizeof DWORD) {
					loopFinished = reallocPidsBuffer(buffSize, pidsVector, pidsArray, bytesReturned);
				}
				else {
					for (auto i = 0; i < (bytesReturned / sizeof DWORD)+1; i++) {
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

	
	String ProcessHelper::getProcessName(HANDLE processHandle) {
		TCHAR nameBuffer[MAX_PATH];
		String procName(L"");
		DWORD result = 0;
 		if(processHandle != nullptr) {
			result = GetModuleBaseName(processHandle, GetModuleHandle(nullptr), nameBuffer, MAX_PATH);
			if(result == 0) {
				setLastError();
			}else {
				procName.append(nameBuffer);
			}
		}
		return procName;
	}

};