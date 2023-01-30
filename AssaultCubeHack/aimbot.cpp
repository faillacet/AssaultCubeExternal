#include "aimbot.h"

Vector3::Vector3() {
	x = y = z = 0;
}

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

PlayerEnt::PlayerEnt(unsigned int selfPointer, Process* procPtr) {
	playerEntPtr = selfPointer;
	proc = procPtr;
}

PlayerEnt::~PlayerEnt() {

}

void PlayerEnt::resolveAddresses() {
	pAddr.health = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.health });
	pAddr.armor = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.armor });
	pAddr.isDead = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.isDead });

	pAddr.headPosX = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.headPosX });
	pAddr.headPosY = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.headPosY });
	pAddr.headPosZ = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.headPosZ });

	pAddr.posX = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.posX });
	pAddr.posY = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.posY });
	pAddr.posZ = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.posZ });

	pAddr.viewX = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.viewX });
	pAddr.viewY = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.viewY });
	pAddr.viewZ = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.viewZ });
}

void PlayerEnt::getAllData() {
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.health, (BYTE*)&health, sizeof(health), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.armor, (BYTE*)&armor, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.isDead, (BYTE*)&isDead, sizeof(isDead), nullptr);

	// Vector3s
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.headPosX, (BYTE*)&headPos.x, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.headPosY, (BYTE*)&headPos.y, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.headPosZ, (BYTE*)&headPos.z, sizeof(armor), nullptr);

	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.posX, (BYTE*)&bodyPos.x, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.posY, (BYTE*)&bodyPos.y, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.posZ, (BYTE*)&bodyPos.z, sizeof(armor), nullptr);

	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.viewX, (BYTE*)&view.x, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.viewY, (BYTE*)&view.y, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.viewZ, (BYTE*)&view.z, sizeof(armor), nullptr);
}

// AIMBOT 

Aimbot::Aimbot(unsigned int modBase, Process* procPtr) {
	proc = procPtr;
	modBaseAddr = modBase;
	playerCountAddr = modBase + OFS.playerCount;

	localPlayer = new PlayerEnt(modBase + OFS.playerent, proc);
	localPlayer->resolveAddresses();

	getPlayerCount();
	unsigned int entListPtr = modBaseAddr + OFS.entityList;
	entListAddr = proc->FindDMAAddress(entListPtr, { 0 });
	entArr = nullptr;
	createEntityList();
}

Aimbot::~Aimbot() {
	if (entArr != nullptr) {
		deleteEntityList();
	}
}

void Aimbot::getPlayerCount() {
	ReadProcessMemory(proc->pHandle, (BYTE*)playerCountAddr, (BYTE*)&playerCount, sizeof(playerCount), nullptr);
}

void Aimbot::createEntityList() {
	if (entArr != nullptr) {
		deleteEntityList();
	}

	entArr = new PlayerEnt*[playerCount-1];
	for (unsigned int i = 1; i < playerCount; i++) {
		entArr[i-1] = new PlayerEnt(entListAddr + (4 * i), proc);
		entArr[i-1]->resolveAddresses();
		entArr[i-1]->getAllData();
		std::cout << entArr[i-1]->health << std::endl;
	}
}

void Aimbot::deleteEntityList() {
	for (unsigned int i = 1; i < playerCount; i++) {
		delete entArr[i-1];
	}
	delete[] entArr;
	entArr = nullptr;
}

void Aimbot::aimLoop() {
	// Update all Data
	localPlayer->getAllData();
	for (unsigned int i = 0; i < playerCount - 1; i++) {
		entArr[i]->getAllData();
	}

	// get closest enemy, then aim at them
	PlayerEnt* closestEnemy = getClosestEnemy();
	aimAt(closestEnemy->headPos);
}

void Aimbot::aimAt(Vector3 targetPos) {
	Vector3 myPos = localPlayer->headPos;
	Vector3 aimAngle = calcAngle(myPos, targetPos);
	WriteProcessMemory(proc->pHandle, (BYTE*)localPlayer->pAddr.viewX, (void*)&aimAngle.x, sizeof(aimAngle.x), nullptr);
	WriteProcessMemory(proc->pHandle, (BYTE*)localPlayer->pAddr.viewY, (void*)&aimAngle.y, sizeof(aimAngle.y), nullptr);
}

float Aimbot::getDistance(Vector3 other) {
	Vector3 myPos = localPlayer->headPos;
	Vector3 delta = Vector3(other.x - myPos.x, other.y - myPos.y, other.z - myPos.z);
	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

float Aimbot::magnitude(Vector3 src) {
	return sqrtf(src.x * src.x + src.y * src.y + src.z * src.z);
}

float Aimbot::distance(Vector3 src, Vector3 dst) {
	Vector3 diff = vecSub(src, dst);
	return magnitude(diff);
}

Vector3 Aimbot::vecSub(Vector3 src, Vector3 dst) {
	Vector3 diff(src.x - dst.x, src.y - dst.y, src.z - dst.z);
	return diff;
}

Vector3 Aimbot::calcAngle(Vector3 src, Vector3 dst) {
	Vector3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / PI * 180.0f + 180.0f;
	angle.y = asinf((dst.z - src.z) / distance(src, dst)) * 180.0f / PI;
	angle.z = 0.0f;
	return angle;
}

PlayerEnt* Aimbot::getClosestEnemy() {
	float minDist = 9999999.0f;
	PlayerEnt* curr = nullptr;
	for (unsigned int i = 0; i < playerCount - 1; i++) {
		if (entArr[i]->isDead) {
			continue;
		}
		float dist = getDistance(entArr[i]->headPos);
		if (dist < minDist) {
			minDist = dist;
			curr = entArr[i];
		}
	}
	return curr;
}