#include "hack.h"

Hack::Hack(Process* tProc) {
	this->tProc = tProc;
	initAddresses();
	getPlayerCount();

	initEntityList(playerCount);
}

Hack::~Hack() {
	if (entList != nullptr) {
		delete entList;
		entList = nullptr;
	}
}

void Hack::updateLocalPlayer() {
	lpAmmo = tProc->parseDMAAddr(dwLocalPlayer, { OFS.playerentOFS.currWepPtr, OFS.playerentOFS.currWepOFS.ammoPtr, 0 });
	lpFirerate = tProc->parseDMAAddr(dwLocalPlayer, { OFS.playerentOFS.currWepPtr, OFS.playerentOFS.currWepOFS.delayPtr, 0 });
}

void Hack::initAddresses() {
	dwModuleBase = tProc->GetModuleBaseAddress(tProc->pId, tProc->pName);
	dwLocalPlayer = dwModuleBase + OFS.playerent;
	dwEntityList = tProc->parseDMAAddr(dwModuleBase + OFS.entityList, { 0 });
	dwPlayerCount = dwModuleBase + OFS.playerCount;
	dwRecoilFunction = dwModuleBase + OFS.recoilFunc;
	lpHealth = tProc->parseDMAAddr(dwLocalPlayer, { OFS.playerentOFS.health });
	lpAmmo = tProc->parseDMAAddr(dwLocalPlayer, { OFS.playerentOFS.currWepOFS.ammoPtr, 0 });
	lpFirerate = tProc->parseDMAAddr(dwLocalPlayer, { OFS.playerentOFS.currWepOFS.delayPtr, 0 });
}

void Hack::initEntityList(int size) {
	entList = new EntityList(tProc, dwLocalPlayer, dwEntityList, size);
}

void Hack::deleteEntityList() {
	if (entList != nullptr) {
		delete entList;
		entList = nullptr;
	}
}

void Hack::update() {
	// check for playercount change
	int oldCount = playerCount;
	ReadProcessMemory(tProc->pHandle, (BYTE*)dwPlayerCount, (BYTE*)&playerCount, sizeof(playerCount), nullptr);
	if (oldCount != playerCount) {
		deleteEntityList();
		initEntityList(playerCount);
		return;
	}

	// else update
	updateLocalPlayer();
	if (entList != nullptr) {
		entList->updateEntities();
	}
}

bool Hack::checkValidEnt(Entity* ent) {
	return true;
}

void Hack::getPlayerCount() {
	ReadProcessMemory(tProc->pHandle, (BYTE*)dwPlayerCount, (BYTE*)&playerCount, sizeof(playerCount), nullptr);
}

// Menu Funcs
const char* Hack::boolToString(bool x) {
	if (x) {
		return "ON";
	}
	return "OFF";
}

void Hack::setWindow() {
	SetConsoleTitleA("Trant's AC Trainer v1.0");
}

void Hack::printToWindow() {
	system("cls");
	std::cout << "----------------------------------------\n"
		<< "        Trant's AC Trainer v1.0         \n"
		<< "----------------------------------------\n"
		<< "            STATUS: Success             \n"
		<< "          Press 'Del' To Exit           \n"
		<< "----------------------------------------\n"
		<< "TOGGLE - FEATURE                STATUS  \n"
		<< "[Home] - Godmode                " << boolToString(bList.bHealth) << "\n"
		<< "[PgUp] - InfiniteAmmo           " << boolToString(bList.bAmmo) << "\n"
		<< "[PgDn] - FireRate               " << boolToString(bList.bFirerate) << "\n"
		<< "[RArr] - No Recoil              " << boolToString(bList.bRecoil) << "\n"
		<< "[DArr] - Aimbot                 " << boolToString(bList.bAimbot) << std::endl;
}

// Loop Handlers
void Hack::getKeyState() {
	// Del Key - EXIT
	if (GetAsyncKeyState(VK_DELETE) & 1) {
		bList.bExit = true;
		bList.bWinUpdate = true;
	}

	// Home Key - HEALTH HACK
	if (GetAsyncKeyState(VK_HOME) & 1) {
		bList.bHealth = !bList.bHealth;
		bList.bWinUpdate = true;
	}

	// Page Up Key - AMMO HACK
	if (GetAsyncKeyState(VK_PRIOR) & 1) {
		bList.bAmmo = !bList.bAmmo;
		bList.bWinUpdate = true;
	}

	// Page Down Key - FIRERATE HACK
	if (GetAsyncKeyState(VK_NEXT) & 1) {
		bList.bFirerate = !bList.bFirerate;
		bList.bWinUpdate = true;
	}

	// Right Arrow Key - NO RECOIL / SPREAD / KICKBACK
	if (GetAsyncKeyState(VK_RIGHT) & 1) {
		bList.bRecoil = !bList.bRecoil;
		bList.bWinUpdate = true;
	}

	if (GetAsyncKeyState(VK_DOWN) & 1) {
		bList.bAimbot = !bList.bAimbot;
		bList.bWinUpdate = true;
		if (bList.bAimbot) {
			//initAimbot();
		}
		else {
			//deleteAimbot();
		}
	}
}

void Hack::executeFeatures() {
	// God Mode
	if (bList.bHealth) {
		WriteProcessMemory(tProc->pHandle, (BYTE*)lpHealth, (BYTE*)&newVal, sizeof(newVal), nullptr);
	}

	// Infinite Ammo
	if (bList.bAmmo) {
		WriteProcessMemory(tProc->pHandle, (BYTE*)lpAmmo, (BYTE*)&newVal, sizeof(newVal), nullptr);
	}

	// Increased fire rate
	if (bList.bFirerate) {
		WriteProcessMemory(tProc->pHandle, (BYTE*)lpFirerate, (BYTE*)&zero, sizeof(zero), nullptr);
	}

	// No Recoil/Spread/Knockback
	if (bList.bRecoil && !bList.bRecoilRe) {
		// save old value + write new
		ReadProcessMemory(tProc->pHandle, (BYTE*)dwRecoilFunction, &oldRecoilOP, sizeof(oldRecoilOP), nullptr);
		WriteProcessMemory(tProc->pHandle, (BYTE*)dwRecoilFunction, &noRecoilOP, sizeof(noRecoilOP), nullptr);
		bList.bRecoilRe = true;
	}
	else if ((!bList.bRecoil || bList.bExit) && bList.bRecoilRe) {
		// replace w/ old value
		WriteProcessMemory(tProc->pHandle, (BYTE*)dwRecoilFunction, &oldRecoilOP, sizeof(oldRecoilOP), nullptr);
		bList.bRecoilRe = false;
	}

	// Basic Aimbot
	if (bList.bAimbot) {
		executeAimbot();
	}
}

bool Hack::getExitStatus() {
	return bList.bExit;
}

bool Hack::getUpdateFlag() {
	return bList.bWinUpdate;
}

void Hack::setUpdateFlag(bool x) {
	bList.bWinUpdate = x;
}

Entity* Hack::getClosestEnemy() {
	float minDist = 999999.0f;
	Entity* curr = nullptr;
	for (int i = 1; i < playerCount; i++) {
		if (entList->entArr[i]->val.isDead || entList->entArr[i]->val.team == entList->entArr[0]->val.team) {
			continue;
		}
		float dist = aMath::getDistance(entList->entArr[0]->val.headPos, entList->entArr[i]->val.headPos);
		if (dist < minDist) {
			minDist = dist;
			curr = entList->entArr[i];
		}
	}
	return curr;
}

void Hack::aimAt(Vec3 targetPos) {
	Vec3 myPos = entList->entArr[0]->val.headPos;
	Vec3 aimAngle = aMath::calcAngle(myPos, targetPos);
	WriteProcessMemory(tProc->pHandle, (BYTE*)entList->entArr[0]->addr.viewX, (void*)&aimAngle.x, sizeof(aimAngle.x), nullptr);
	WriteProcessMemory(tProc->pHandle, (BYTE*)entList->entArr[0]->addr.viewY, (void*)&aimAngle.y, sizeof(aimAngle.y), nullptr);
}

void Hack::executeAimbot() {
	if (entList == nullptr) return;

	Entity* closestEnemy = getClosestEnemy();
	if (closestEnemy != nullptr) {
		aimAt(closestEnemy->val.headPos);
	}
}