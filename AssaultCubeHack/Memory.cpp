#include "Memory.h"

void memory::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess) 
{
	DWORD oldProtect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);
}

void memory::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess) 
{
	// set each byte to noop instruction
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);
	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}