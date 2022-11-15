#include <iostream>
#include "Process.h"
#include "Memory.h"

#define ENTITYOFFSET 0x18AC00

int main()
{
	// Define Target Process And Attach
	Process tProc = Process(L"ac_client.exe");
	bool status = tProc.attachToProcess();
	if (!status) {
		std::cout << "ERROR ATTACHING TO PROCESS, NOW TERMINATING..." << std::endl;
		return 0;
	}
	std::cout << "SUCCESSFULLY ATTACHED TO PROCESS" << std::endl;


	// CODE NOT REFACTORED PAST THIS POINT ---------------------------------------------------------- TODO: REFACTOR VERI DIRTY


	// Find Needed Addresses
	std::vector<unsigned int> healthOffset = { 0xEC };
	std::vector<unsigned int> ammoOffset = { 0x140 };

	unsigned int moduleBaseAddr = tProc.GetModuleBaseAddress(tProc.pId, tProc.pName);
	unsigned int localPlayerPtr = moduleBaseAddr + ENTITYOFFSET;
	unsigned int healthAddr = tProc.FindDMAAddress(tProc.pHandle, localPlayerPtr, healthOffset);
	unsigned int ammoAddr = tProc.FindDMAAddress(tProc.pHandle, localPlayerPtr, ammoOffset);

	// Main Loop ------------------------------------
	bool bHealth = false;
	bool bAmmo = false;

	const unsigned int newVal = 1337;


	while (true) {
		// Home Key
		if (GetAsyncKeyState(VK_HOME) & 1) {
			bHealth = !bHealth;
			if (bHealth) {
				std::cout << "HEALTH HACK ENABLED" << std::endl;
			}
			else {
				std::cout << "HEALTH HACK DISABLED" << std::endl;
			}
		}
		// Page Up Key
		if (GetAsyncKeyState(VK_PRIOR) & 1) {
			bAmmo = !bAmmo;
			if (bAmmo) {
				std::cout << "AMMO HACK ENABLED" << std::endl;
			}
			else {
				std::cout << "AMMO HACK DISABLED" << std::endl;
			}
		}
		// Del Key
		if (GetAsyncKeyState(VK_DELETE) & 1) {
			break;
		}

		// Toggled Features
		if (bHealth) {
			//memory::PatchEx((BYTE*)healthAddr, (BYTE*)&newVal, sizeof(newVal), hProcess);
			WriteProcessMemory(tProc.pHandle, (BYTE*)healthAddr, (BYTE*)&newVal, sizeof(newVal), nullptr);
		}
		if (bAmmo) {
			//memory::PatchEx((BYTE*)ammoAddr, (BYTE*)&newVal, sizeof(newVal), hProcess);
			WriteProcessMemory(tProc.pHandle, (BYTE*)ammoAddr, (BYTE*)&newVal, sizeof(newVal), nullptr);
		}

		Sleep(5);
	}

	std::cout << "NOW TERMINATING PROGRAM..." << std::endl;
	return 0;
}
