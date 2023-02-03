#pragma once
#include "includes.h"
#include "Process.h"

class EntityAddresses {
public:
	uintptr_t base;

	uintptr_t headPosX, headPosY, headPosZ;
	uintptr_t bodyPosX, bodyPosY, bodyPosZ;
	uintptr_t viewX, viewY, viewZ;

	uintptr_t health;
	uintptr_t armor;
	uintptr_t team;
	uintptr_t isDead;
};

class EntityValues {
public:
	Vec3 headPos;
	Vec3 bodyPos;
	Vec3 view;

	unsigned int health;
	unsigned int armor;
	unsigned int team;
	bool isDead;
};

class Entity {
public:
	Process* tProc = nullptr;
	EntityAddresses addr;
	EntityValues val;

	Entity(Process* tProc, uintptr_t entBaseAddr);
	~Entity();

	void resolveAddresses();
	void updateValues();
};

class EntityList {
public:
	Process* tProc = nullptr;
	Entity** entArr = nullptr;
	uintptr_t entListAddr = NULL;
	int size = NULL;

	EntityList(Process* tProc, uintptr_t localPlayerAddr, uintptr_t entListAddr, int size);
	~EntityList();

	void updateEntities();
};