#include "entity.h"
#include <iostream>

// ENTITY
Entity::Entity(Process* tProc, uintptr_t entBaseAddr) {
	this->tProc = tProc;
	this->addr.base = entBaseAddr;
	resolveAddresses();
	updateValues();
}

Entity::~Entity() {

}

void Entity::resolveAddresses() {
	addr.health = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.health });
	addr.armor = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.armor });
	addr.team = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.team });
	addr.isDead = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.isDead });

	addr.headPosX = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.headPosX });
	addr.headPosY = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.headPosY });
	addr.headPosZ = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.headPosZ });

	addr.bodyPosX = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.posX });
	addr.bodyPosY = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.posY });
	addr.bodyPosZ = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.posZ });

	addr.viewX = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.viewX });
	addr.viewY = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.viewY });
	addr.viewZ = tProc->parseDMAAddr(addr.base, { OFS.playerentOFS.viewZ });
}

void Entity::updateValues() {
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.health, (BYTE*)&val.health, sizeof(val.health), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.armor, (BYTE*)&val.armor, sizeof(val.armor), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.team, (BYTE*)&val.team, sizeof(val.team), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.isDead, (BYTE*)&val.isDead, sizeof(val.isDead), nullptr);

	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.headPosX, (BYTE*)&val.headPos.x, sizeof(val.headPos.x), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.headPosY, (BYTE*)&val.headPos.y, sizeof(val.headPos.y), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.headPosZ, (BYTE*)&val.headPos.z, sizeof(val.headPos.z), nullptr);

	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.bodyPosX, (BYTE*)&val.bodyPos.x, sizeof(val.bodyPos.x), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.bodyPosY, (BYTE*)&val.bodyPos.y, sizeof(val.bodyPos.y), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.bodyPosZ, (BYTE*)&val.bodyPos.z, sizeof(val.bodyPos.z), nullptr);

	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.viewX, (BYTE*)&val.view.x, sizeof(val.view.x), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.viewY, (BYTE*)&val.view.y, sizeof(val.view.y), nullptr);
	ReadProcessMemory(tProc->pHandle, (BYTE*)addr.viewZ, (BYTE*)&val.view.z, sizeof(val.view.z), nullptr);
}

// ENTITY LIST 
EntityList::EntityList(Process* tProc, uintptr_t localPlayerAddr, uintptr_t entListAddr, int size) {
	this->tProc = tProc;
	this->entListAddr = entListAddr;
	this->size = size;

	// Populate array - LocalPlayer, then the rest
	entArr = new Entity*[size];
	entArr[0] = new Entity(tProc, localPlayerAddr);
	for (int i = 1; i < size; i++) {
		entArr[i] = new Entity(tProc, entListAddr + (4 * i));
	}
}

EntityList::~EntityList() {
	for (int i = 0; i < size; i++) {
		//std::cout << entArr[i]->val.team << std::endl;
		std::cout << std::dec << entArr[i]->val.health << std::endl;
		delete entArr[i];
	}
	delete[] entArr;
	entArr = nullptr;
}

void EntityList::updateEntities() {
	for (int i = 0; i < size; i++) {
		entArr[i]->updateValues();
	}
}