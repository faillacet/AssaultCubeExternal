#pragma once

#include <Windows.h>
#include <iostream>
#include "Process.h"

struct Vector3 { float x, y, z; };

class PlayerEnt
{
public:
	unsigned int playerEntPointer;
	Process* proc;
	Vector3 headPos; //0x0004
	Vector3 bodyPos; //0x0028
	Vector3 view; //0x0034
	unsigned int health; //0x00EC
	unsigned int armor; //0x00F0
	char* name[15]; // 0x205
	//class N000002AE* prevWeapon; //0x0360
	//class N0000029A* currWeapon; //0x0364
	//class N00000286* nextWeapon; //0x0368

	PlayerEnt(unsigned int playerEntPointer, Process* proc);
	~PlayerEnt();
	// void resolveAddresses();
	//void getHeadPos();
	void getBodyPos();
	//void getView();
	void getHealth();
	//void getArmor();
	void getName();
};

