#include "aimbot.h"

PlayerEnt::PlayerEnt(unsigned int selfPointer, Process* procPtr) {
	playerEntPointer = selfPointer;
	proc = procPtr;
}

PlayerEnt::~PlayerEnt() {

}

void PlayerEnt::getBodyPos() {
	std::cout << "BodyPos: " << std::endl;
}

void PlayerEnt::getHealth() {
	unsigned int healthAddr = proc->FindDMAAddress(playerEntPointer, { 0xEC });
	ReadProcessMemory(proc->pHandle, (BYTE*)healthAddr, &health, sizeof(health), nullptr);
	std::cout << health << std::endl;
}

void PlayerEnt::getName() {
	unsigned int nameAddr = proc->FindDMAAddress(playerEntPointer, { 0x205 });
	ReadProcessMemory(proc->pHandle, (BYTE*)nameAddr, &name, sizeof(name), nullptr);
	std::cout << name << std::endl;
}