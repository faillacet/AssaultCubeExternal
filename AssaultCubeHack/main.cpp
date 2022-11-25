#include <iostream>
#include <array>
#include "Process.h"
#include "Memory.h"
#include "offsets.h"

// returns true on exit
bool getKeyState(bool &bHealth, bool &bAmmo, bool &bDelay, bool &bRecoil) {
	// Del Key - EXIT
	if (GetAsyncKeyState(VK_DELETE) & 1) {
		return true;
	}

	// Home Key - HEALTH HACK
	if (GetAsyncKeyState(VK_HOME) & 1) {
		bHealth = !bHealth;
		if (bHealth) {
			std::cout << "HEALTH HACK ENABLED" << std::endl;
		}
		else {
			std::cout << "HEALTH HACK DISABLED" << std::endl;
		}
	}

	// Page Up Key - AMMO HACK
	if (GetAsyncKeyState(VK_PRIOR) & 1) {
		bAmmo = !bAmmo;
		if (bAmmo) {
			std::cout << "AMMO HACK ENABLED" << std::endl;
		}
		else {
			std::cout << "AMMO HACK DISABLED" << std::endl;
		}
	}

	// Page Down Key - FIRERATE HACK
	if (GetAsyncKeyState(VK_NEXT) & 1) {
		bDelay = !bDelay;
		if (bDelay) {
			std::cout << "FIRERATE HACK ENABLED" << std::endl;
		}
		else {
			std::cout << "FIRERATE HACK DISABLED" << std::endl;
		}
	}

	// Right Arrow Key - NO RECOIL / SPREAD / KICKBACK
	if (GetAsyncKeyState(VK_RIGHT) & 1) {
		bRecoil = !bRecoil;
		if (bRecoil) {
			std::cout << "NO RECOIL / SPREAD / KICKBACK HACK ENABLED" << std::endl;
		}
		else {
			std::cout << "NO RECOIL / SPREAD / KICKBACK HACK DISABLED" << std::endl;
		}
	}
	
	return false;
}

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

	// Get Addresses
	unsigned int moduleBaseAddr = tProc.GetModuleBaseAddress(tProc.pId, tProc.pName);
	unsigned int localPlayerPtr = moduleBaseAddr + OFS.playerent;
	unsigned int recoilFuncAddr = moduleBaseAddr + OFS.recoilFunc;
	unsigned int healthAddr = tProc.FindDMAAddress(localPlayerPtr, { OFS.playerentOFS.health });


	// Main Loop ------------------------------------
	std::array<BYTE, 3> oldRecoilOP = { 0x00, 0x00, 0x00 };
	std::array<BYTE, 3> noRecoilOP = { 0xC2, 0x08, 0x00 };
	const unsigned int newVal = 1337;
	const unsigned int zero = 0;
	bool bHealth = false;
	bool bAmmo = false;
	bool bDealy = false;

	bool bRecoil = false;
	bool bRecoilFlag = false;

	bool exitProgram = false;

	do {
		// On return false, terminate program
		if (getKeyState(bHealth, bAmmo, bDealy, bRecoil)) {
			exitProgram = true;
		}
		
		// Trainer Features - TODO ORGANIZE INTO CLASS + ON EXIT CLEANUP FUNCTION
		// God Mode
		if (bHealth) {
			WriteProcessMemory(tProc.pHandle, (BYTE*)healthAddr, (BYTE*)&newVal, sizeof(newVal), nullptr);
		}

		// Infinite Ammo
		if (bAmmo) {
			unsigned int ammoAddr = tProc.FindDMAAddress(localPlayerPtr, { OFS.playerentOFS.currWepPtr, OFS.playerentOFS.currWepOFS.ammoPtr, 0 });
			WriteProcessMemory(tProc.pHandle, (BYTE*)ammoAddr, (BYTE*)&newVal, sizeof(newVal), nullptr);
		}

		// Increased fire rate
		if (bDealy) {
			unsigned int delayAddr = tProc.FindDMAAddress(localPlayerPtr, { OFS.playerentOFS.currWepPtr, OFS.playerentOFS.currWepOFS.delayPtr, 0 });
			WriteProcessMemory(tProc.pHandle, (BYTE*)delayAddr, (BYTE*)&zero, sizeof(zero), nullptr);
		}

		// No Recoil 
		if (bRecoil && !bRecoilFlag) {
			// save old value
			ReadProcessMemory(tProc.pHandle, (BYTE*)recoilFuncAddr, &oldRecoilOP, sizeof(oldRecoilOP), nullptr);
			// overwrite to new value
			WriteProcessMemory(tProc.pHandle, (BYTE*)recoilFuncAddr, &noRecoilOP, sizeof(noRecoilOP), nullptr);
			bRecoilFlag = true;
		}
		else if ((!bRecoil && bRecoilFlag) || exitProgram) {
			// replace w/ old value
			WriteProcessMemory(tProc.pHandle, (BYTE*)recoilFuncAddr, &oldRecoilOP, sizeof(oldRecoilOP), nullptr);
			bRecoilFlag = false;
		}
		Sleep(5);

	} while (!exitProgram);

	std::cout << "NOW TERMINATING PROGRAM..." << std::endl;
	return 0;
}