#include "aimbot.h"

PlayerEnt::PlayerEnt(unsigned int selfPointer, Process* procPtr) {
	playerEntPtr = selfPointer;
	proc = procPtr;
}

PlayerEnt::~PlayerEnt() {

}

void PlayerEnt::resolveAddresses() {
	pAddr.health = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.health });
	pAddr.armor = proc->FindDMAAddress(playerEntPtr, { OFS.playerentOFS.armor });

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

	// Vector3s
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.headPosX, (BYTE*)&headPos.x, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.headPosY, (BYTE*)&headPos.y, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.headPosZ, (BYTE*)&headPos.z, sizeof(armor), nullptr);

	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.posX, (BYTE*)&bodyPos.x, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.posY, (BYTE*)&bodyPos.y, sizeof(armor), nullptr);
	ReadProcessMemory(proc->pHandle, (BYTE*)pAddr.posZ, (BYTE*)&bodyPos.z, sizeof(armor), nullptr);

	// LEAVING OUT VIEW FOR NOW INFO NOT NEEDED
}