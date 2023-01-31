#pragma once

#include <Windows.h>
#include <iostream>
#include "Process.h"
#include "offsets.h"

class Vector3 { 
public:
	float x, y, z; 

	Vector3();
	Vector3(float x, float y, float z);
};

struct playerAddresses {
	unsigned int headPosX, headPosY, headPosZ;
	unsigned int posX, posY, posZ;
	unsigned int viewX, viewY, viewZ;
	unsigned int health;
	unsigned int armor;
	unsigned int team;
	unsigned int isDead;
};

// TODO MIGRATE TO VALUES IN A STRUCT AS WELL (makes things cleaner)
struct playerValues {
	Vector3 headPos;
	Vector3 bodyPos;
	Vector3 view;
	unsigned int health;
	unsigned int armor;
	unsigned int team;
	bool isDead;
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
	unsigned int team;
	bool isDead; // 0 is alive, 1 is dead
	//char* name[15]; // 0x205

	// Functions
	PlayerEnt(unsigned int playerEntAddr, Process* proc);
	~PlayerEnt();

	void resolveAddresses();
	void getAllData();
	//void getName();
};

#define PI 3.1415927f

class Aimbot {
public:
	Process* proc;
	unsigned int modBaseAddr;
	unsigned int playerCountAddr;
	unsigned int entListAddr;	// First entity is at + 4

	PlayerEnt** entArr = nullptr;
	PlayerEnt** enemyArr = nullptr;
	PlayerEnt* localPlayer = nullptr;

	unsigned int playerCount;

	// Setup
	Aimbot(unsigned int modBase, Process* proc);
	~Aimbot();
	void getPlayerCount();
	void createEntityList();
	void deleteEntityList();

	// Math Calcs
	float getDistance(Vector3 other);
	float magnitude(Vector3 src);
	float distance(Vector3 src, Vector3 dst);
	Vector3 vecSub(Vector3 src, Vector3 dst);
	Vector3 calcAngle(Vector3 src, Vector3 dst);
	PlayerEnt* getClosestEnemy();

	// Loop
	void aimLoop();
	void aimAt(Vector3 targetPos);
};
