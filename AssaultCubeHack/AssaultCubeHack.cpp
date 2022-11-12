#include <iostream>
#include <vector>
#include <Windows.h>

// Move later
#include <TlHelp32.h>

// Functions
DWORD GetProcId(const wchar_t* procName)
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

unsigned int GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
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

unsigned int FindDMAAddress(HANDLE h, unsigned int ptr, std::vector<unsigned int> offsets)
{
	unsigned int addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(h, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

#define ENTITYOFFSET 0x18AC00

int main()
{
	// Get Process Id
	DWORD procId = GetProcId(L"ac_client.exe");

	// Get Module Base Address
	unsigned int moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");
	 
	// Create Handle to Process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	// Pointer Handling
	unsigned int dynamicPtrBaseAddr = moduleBase + ENTITYOFFSET;
	std::vector<unsigned int> ammoOffsets = { 0x140 };
	std::vector<unsigned int> healthOffsets = { 0xEC };

	// ACTUALY EDITING OF MEMORY ------------------------------

	// Find and Edit Ammo Value
	unsigned int ammoVal = 0;
	unsigned int ammoAddr = FindDMAAddress(hProcess, dynamicPtrBaseAddr, ammoOffsets);
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoVal, sizeof(ammoVal), nullptr);

	unsigned int newAmmo = 1337;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

	// Find and Edit Health Value
	unsigned int healthVal = 0;
	unsigned int healthAddr = FindDMAAddress(hProcess, dynamicPtrBaseAddr, healthOffsets);
	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthVal, sizeof(healthVal), nullptr);

	unsigned int newHealth = 1337;
	WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);


	// Process Exit
	getchar();
	std::cout << "Now Terminating Process...\n";
	return 0;
}






//std::cout << "DynamicPtrBaseAddress: 0x" << std::hex << dynamicPtrBaseAddr << std::endl;
//std::cout << "AmmoAddress: 0x" << std::hex << ammoAddr << std::endl;
//std::cout << "CurrentAmmo: " << std::dec << ammoVal << std::endl;