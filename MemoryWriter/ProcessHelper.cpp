#include "stdafx.h"
#include "ProcessHelper.h"
#include "plexerCodeUtil.h"
#include "ProcessConstants.h"

#define P_HELPER_SLASH  L"/\\"


namespace plexerCode {

	DWORD ProcessHelper::lastError_ = 0;

	/*
	Searches all processes and returns a handle to a process that matches procName with PROCESS_ALL_ACCESS
	@procName The name of the process
	@return The handle to the process or nullptr
	Note: If multiple processes have the same name, the first process encountered is returned.
	*/
	Process* ProcessHelper::getProcessByName(const PC_CHAR* procName) {
		//TODO refactor
		LOG(DEBUG) << "Searching for process handle for " << procName;
		auto pids = getAllProcPids();
		auto found = false;
		Process *result = nullptr;
		PC_HANDLE handle;
		if (pids) {
			for (auto pid = (*pids).begin(); pid != (*pids).end(); ++pid) {
				handle = getProcessByPid(PC_READ_ACCESS, *pid);
				if(handle != nullptr) {
					found = compareFileNames(result, procName);
				}
				if(found) {
					LOG(DEBUG) << "Found matching process for " << procName << " with PID " << *pid;
					CloseHandle(handle);
					handle = getProcessByPid(PC_FULL_ACCESS,*pid);
					break;
				}
			}
		}
		if(!found) {
			LOG(DEBUG) << "Didn't find matching process for " << procName;
			return nullptr;
		}
		return result;
	}

	//TODO refactor
	bool ProcessHelper::compareFileNames(PC_HANDLE result,const PC_CHAR* fileName) {
		PC_CHAR name[MAX_PATH];
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
		pidsArray = util::initDWord(buffSize);
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
	std::shared_ptr<std::vector<PC_PID>> ProcessHelper::getAllProcPids() {
		auto buffSize = BUFF_SIZE;
		auto pidsVector = std::make_shared<std::vector<PC_PID>>();
		auto pidsArray = util::initDWord(buffSize);
		DWORD bytesReturned = 0;
		auto result = EnumProcesses(pidsArray, buffSize, &bytesReturned);
		if (!result) {
			cleanGetAllProcPids(nullptr, pidsArray, true);
			LOG(ERROR) << "Process enumeration failed: " << ProcessConstants::errorToString(getLastError());
		}
		else {
			auto loopFinished = false;
			while (!loopFinished) {
				if (bytesReturned >= buffSize * sizeof PC_PID) {
					loopFinished = reallocPidsBuffer(buffSize, nullptr, pidsArray, bytesReturned);
				}
				else {
					for (unsigned int i = 0; i < (bytesReturned / sizeof PC_PID)+1; i++) {
						pidsVector->push_back(pidsArray[i]);
					}
					loopFinished = true;
				}
			}
		}
		cleanGetAllProcPids(nullptr, pidsArray, false);
		return pidsVector;
	}


	void ProcessHelper::cleanGetAllProcPids(std::vector<PC_PID>* pidsVector, PC_PID* pidsArray, bool bail) {
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

	void ProcessHelper::closeHandle(HANDLE handle) {
		CloseHandle(handle);
	}

	/*
	Returns the full Windows name of the specified handle
	@processHandle process to check
	@return The full path (e.g. C:\full\path\to\program.exe) or an empty string if failure.
	*/
	String ProcessHelper::getProcessName(PC_HANDLE processHandle) {
		TCHAR nameBuffer[MAX_PATH];
		DWORD nameBufferSize = MAX_PATH;
		String procName(L"");
		DWORD result = 0;
 		if(processHandle != nullptr) {
			result = QueryFullProcessImageName(processHandle,0,nameBuffer, &nameBufferSize);
			if(result == 0) {
				setLastError();
			}else {
				procName.append(nameBuffer);
			}
		}
		return procName;
	}

	
	/*
	Returns process handle with desired access level
	@desiredAccess Desired access level
	@pid PID of process
	@return Handle to process, or null. If failure occurs, last error is set.
	*/
	PC_HANDLE ProcessHelper::getProcessByPid(PC_ACCESS_TYPE desiredAccess, PC_PID pid) {
		auto process = OpenProcess(desiredAccess, true, pid);
		if(process == nullptr) {
			setLastError();
			LOG(ERROR) << "Failed to open process with PID " << pid << ": " << ProcessConstants::errorToString(getLastError());
		}
		return process;
	}

	DWORD ProcessHelper::getProcessId(HANDLE procHandle) {
		return GetProcessId(procHandle);
	}
};