#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

class Process {
public:
	const wchar_t* pName;
	DWORD pId;
	HANDLE pHandle;

	Process(const wchar_t* processName);
	~Process();

	bool attachToProcess();
	DWORD GetProcId(const wchar_t* procName);
	unsigned int GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
	unsigned int FindDMAAddress(HANDLE h, unsigned int ptr, std::vector<unsigned int> offsets);
};