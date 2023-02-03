#pragma once
#include "includes.h"
#include "Process.h"
#include "entity.h"
#include "aMath.h"
#include <array>
#include <iostream>

class boolList {
public:
	bool bWinUpdate = false;
	bool bExit = false;
	bool bHealth = false;
	bool bAmmo = false;
	bool bFirerate = false;
	bool bRecoil = false;
	bool bRecoilRe = false;
	bool bAimbot = false;
};

class Hack {
public:
	Process* tProc = nullptr;
	EntityList* entList = nullptr;
	int playerCount = NULL;
	boolList bList;

	// Addresses
	uintptr_t dwModuleBase = NULL;
	uintptr_t dwLocalPlayer = NULL;
	uintptr_t dwEntityList = NULL;
	uintptr_t dwPlayerCount = NULL;
	uintptr_t dwRecoilFunction = NULL;

	uintptr_t lpHealth;
	uintptr_t lpAmmo;
	uintptr_t lpFirerate;

	// OP Code / Buffers
	std::array<BYTE, 3> oldRecoilOP = { 0x00, 0x00, 0x00 };
	std::array<BYTE, 3> noRecoilOP = { 0xC2, 0x08, 0x00 };
	const unsigned int newVal = 1337;
	const unsigned int zero = 0;

	// Funcs
	Hack(Process* tProc);
	~Hack();

	// Setup
	void initAddresses();
	void initEntityList(int size);
	void deleteEntityList();

	void update();
	void updateLocalPlayer();
	bool checkValidEnt(Entity* ent);
	void getPlayerCount();

	// Menu
	const char* boolToString(bool x);
	void setWindow();
	void printToWindow();

	// Loop Handlers
	void getKeyState();
	bool getExitStatus();
	bool getUpdateFlag();
	void setUpdateFlag(bool x);
	void executeFeatures();

	// Aimbot
	Entity* getClosestEnemy();
	void aimAt(Vec3 targetPos);
	void executeAimbot();
};

