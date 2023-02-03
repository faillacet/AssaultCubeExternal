#pragma once
#include "includes.h"

class Process {
public:
	bool status;
	const wchar_t* pName;
	DWORD pId;
	HANDLE pHandle;
	unsigned int baseAddress;

	Process(const wchar_t* processName);
	~Process();

	bool attachToProcess();
	DWORD GetProcId(const wchar_t* procName);
	unsigned int GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
	uintptr_t parseDMAAddr(uintptr_t addr, std::vector<uintptr_t> offsets);
	//unsigned int FindDMAAddress(unsigned int ptr, std::vector<unsigned int> offsets);
};