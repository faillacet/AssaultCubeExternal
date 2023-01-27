#pragma once

#include <Windows.h>
#include <iostream>
#include "Process.h"
#include "offsets.h"

struct Vector3 { float x, y, z; };

struct playerAddresses {
	unsigned int headPosX, headPosY, headPosZ;
	unsigned int posX, posY, posZ;
	unsigned int viewX, viewY, viewZ;
	unsigned int health;
	unsigned int armor;
};

class PlayerEnt
{
public:
	Process* proc;
	unsigned int playerEntPtr;
	playerAddresses pAddr;

	Vector3 headPos; //0x0004
	Vector3 bodyPos; //0x0028
	Vector3 view; //0x0034
	unsigned int health; //0x00EC
	unsigned int armor; //0x00F0
	//char* name[15]; // 0x205

	// Functions
	PlayerEnt(unsigned int playerEntAddr, Process* proc);
	~PlayerEnt();

	void resolveAddresses();
	void getAllData();
	//void getName();
};

class Aimbot {
public:
	bool enabled;

	//void resolveAddresses(PlayerEnt[] );
};
