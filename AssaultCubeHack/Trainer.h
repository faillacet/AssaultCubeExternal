#pragma once

#include <Windows.h>
#include <array>
#include <iostream>
#include "Process.h"
#include "offsets.h"
#include "aimbot.h"

// Globals
const unsigned int newVal = 1337;
const unsigned int zero = 0;

struct {
	unsigned int moduleBase;
	unsigned int playerPtr;
	unsigned int recoilFunc;

	// Feature Specific
	unsigned int health;
	unsigned int ammo;
	unsigned int firerate;
} localAddr; 

class Trainer {
private:
	Process* tProc;

	// KEY STATES
	bool bWinUpdate = false;
	bool bExit = false;
	bool bHealth = false;
	bool bAmmo = false;
	bool bFirerate = false;
	bool bRecoil = false;
	bool bRecoilRe = false;

	// OP Code / Buffers
	std::array<BYTE, 3> oldRecoilOP = { 0x00, 0x00, 0x00 };
	std::array<BYTE, 3> noRecoilOP = { 0xC2, 0x08, 0x00 };

public:
	
	
	// Functions -------------
	Trainer(Process* procPtr);
	~Trainer();
	void initAddresses();

	const char* boolToString(bool x);
	void setWindow();
	void printToWindow();

	void updateAddresses();
	//void updateGameData();
	void executeFeatures();
	void getKeyState();
	bool getExitStatus();
	bool getUpdateFlag();
	void setUpdateFlag(bool x);

	void TESTFUNC();
};