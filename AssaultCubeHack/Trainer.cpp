#include "Trainer.h"

Trainer::Trainer(Process* procPtr) {
	tProc = procPtr;
	initAddresses();
}

Trainer::~Trainer() {

}

const char* Trainer::boolToString(bool x) {
	if (x) {
		return "ON";
	}
	return "OFF";
}

void Trainer::setWindow() {
	SetConsoleTitleA("Trant's AC Trainer v1.0");
}

void Trainer::printToWindow() {
	system("cls");
	std::cout << "----------------------------------------\n"
		<< "        Trant's AC Trainer v1.0         \n"
		<< "----------------------------------------\n"
		<< "            STATUS: Success             \n"
		<< "          Press 'Del' To Exit           \n"
		<< "----------------------------------------\n"
		<< "TOGGLE - FEATURE                STATUS  \n"
		<< "[Home] - Godmode                " << boolToString(bHealth) << "\n"
		<< "[PgUp] - InfiniteAmmo           " << boolToString(bAmmo) << "\n"
		<< "[PgDn] - FireRate               " << boolToString(bFirerate) << "\n"
		<< "[RArr] - No Recoil              " << boolToString(bRecoil) << "\n"
		<< "[DArr] - Aimbot                 " << boolToString(bAimbot) << std::endl;
}

void Trainer::initAddresses() {
	localAddr.moduleBase = tProc->GetModuleBaseAddress(tProc->pId, tProc->pName);
	localAddr.playerPtr = localAddr.moduleBase + OFS.playerent;
	localAddr.recoilFunc = localAddr.moduleBase + OFS.recoilFunc;
	localAddr.health = tProc->FindDMAAddress(localAddr.playerPtr, { OFS.playerentOFS.health });
}

void Trainer::updateAddresses() {	
	// NEED UPDATES
	localAddr.ammo = tProc->FindDMAAddress(localAddr.playerPtr, { OFS.playerentOFS.currWepPtr, OFS.playerentOFS.currWepOFS.ammoPtr, 0 });
	localAddr.firerate = tProc->FindDMAAddress(localAddr.playerPtr, { OFS.playerentOFS.currWepPtr, OFS.playerentOFS.currWepOFS.delayPtr, 0 });
}

void Trainer::getKeyState() {
	// Del Key - EXIT
	if (GetAsyncKeyState(VK_DELETE) & 1) {
		bExit = true;
		bWinUpdate = true;
	}

	// Home Key - HEALTH HACK
	if (GetAsyncKeyState(VK_HOME) & 1) {
		bHealth = !bHealth;
		bWinUpdate = true;
	}

	// Page Up Key - AMMO HACK
	if (GetAsyncKeyState(VK_PRIOR) & 1) {
		bAmmo = !bAmmo;
		bWinUpdate = true;
	}

	// Page Down Key - FIRERATE HACK
	if (GetAsyncKeyState(VK_NEXT) & 1) {
		bFirerate = !bFirerate;
		bWinUpdate = true;
	}

	// Right Arrow Key - NO RECOIL / SPREAD / KICKBACK
	if (GetAsyncKeyState(VK_RIGHT) & 1) {
		bRecoil = !bRecoil;
		bWinUpdate = true;
	}

	if (GetAsyncKeyState(VK_DOWN) & 1) {
		bAimbot = !bAimbot;
		bWinUpdate = true;
		if (bAimbot) {
			initAimbot();
		}
		else {
			deleteAimbot();
		}
	}
}

void Trainer::executeFeatures() {
	// God Mode
	if (bHealth) {
		WriteProcessMemory(tProc->pHandle, (BYTE*)localAddr.health, (BYTE*)&newVal, sizeof(newVal), nullptr);
	}

	// Infinite Ammo
	if (bAmmo) {
		WriteProcessMemory(tProc->pHandle, (BYTE*)localAddr.ammo, (BYTE*)&newVal, sizeof(newVal), nullptr);
	}

	// Increased fire rate
	if (bFirerate) {
		WriteProcessMemory(tProc->pHandle, (BYTE*)localAddr.firerate, (BYTE*)&zero, sizeof(zero), nullptr);
	}

	// No Recoil/Spread/Knockback
	if (bRecoil && !bRecoilRe) {
		// save old value + write new
		ReadProcessMemory(tProc->pHandle, (BYTE*)localAddr.recoilFunc, &oldRecoilOP, sizeof(oldRecoilOP), nullptr);
		WriteProcessMemory(tProc->pHandle, (BYTE*)localAddr.recoilFunc, &noRecoilOP, sizeof(noRecoilOP), nullptr);
		bRecoilRe = true;
	}
	else if ((!bRecoil || bExit) && bRecoilRe) {
		// replace w/ old value
		WriteProcessMemory(tProc->pHandle, (BYTE*)localAddr.recoilFunc, &oldRecoilOP, sizeof(oldRecoilOP), nullptr);
		bRecoilRe = false;
	}

	// Basic Aimbot
	if (bAimbot) {
		executeAimbot();
	}
}

bool Trainer::getExitStatus() {
	return bExit;
}

bool Trainer::getUpdateFlag() {
	return bWinUpdate;
}

void Trainer::setUpdateFlag(bool x) {
	bWinUpdate = x;
}

void Trainer::initAimbot() {
	aimbot = new Aimbot(localAddr.moduleBase, tProc);
}

void Trainer::executeAimbot() {
	aimbot->aimLoop();
}

void Trainer::deleteAimbot() {
	if (aimbot != nullptr) {
		delete aimbot;
	}
}