#include "Process.h"

// Process Constructor (Handles Setup)
Process::Process(const wchar_t* processName) {
	pName = processName;
	pId = 0;
	pHandle = NULL;
	baseAddress = NULL;

	// Attempt Attach
	status = attachToProcess();
}

Process::~Process() {
	if (pHandle != NULL && pHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(pHandle);
	}
}

bool Process::attachToProcess()
{
	// Get Process Id
	pId = GetProcId(pName);
	if (!pId) {
		//::cout << "PROCESS NOT FOUND" << std::endl;
		return false;
	}

	// Create Handle to Target Process
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pId);
	if (pHandle == INVALID_HANDLE_VALUE || pHandle == 0) {
		//std::cout << "FAILED TO OPEN HANDLE TO PROCCESS" << std::endl;
		return false;
	}

	// Get Module (Process) Base Address
	baseAddress = GetModuleBaseAddress(pId, pName);

	return true;
}

DWORD Process::GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(h, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(h, &procEntry));
		}
	}
	CloseHandle(h);
	return procId;
}

unsigned int Process::GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	unsigned int modBaseAddr = 0;
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (h != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(h, &modEntry)) {
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(h, &modEntry));
		}
	}
	CloseHandle(h);
	return modBaseAddr;
}

uintptr_t Process::parseDMAAddr(uintptr_t addr, std::vector<uintptr_t> offsets) {
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(pHandle, (byte*)addr, &addr, sizeof(unsigned int), nullptr);
		addr += offsets[i];
	}
	return addr;
}