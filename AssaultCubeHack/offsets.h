#pragma once
#include <cstdint>

struct {
	uintptr_t recoilFunc = 0xC8BA0;
	uintptr_t playerCount = 0x18AC0C;
	uintptr_t entityList = 0x18AC04; // first entity at + 4
	uintptr_t playerent = 0x18AC00;

	// offsets relative to playerent
	struct {
		uintptr_t headPosX = 0x4;
		uintptr_t headPosY = 0x8;
		uintptr_t headPosZ = 0xC;
		uintptr_t posX = 0x28;
		uintptr_t posY = 0x2C;
		uintptr_t posZ = 0x30;
		uintptr_t viewX = 0x34;
		uintptr_t viewY = 0x38;
		uintptr_t viewZ = 0x3C;
		uintptr_t health = 0xEC;
		uintptr_t armor = 0xF0;
		uintptr_t isDead = 0x318; // 0 - alive, 1 - dead
		uintptr_t team = 0x30C;	// single byte

		uintptr_t currWepPtr = 0x364;
		// offsets relative to currWepPtr
		struct {
			uintptr_t id = 0x4;
			uintptr_t ammoReservePtr = 0x10;
			uintptr_t ammoPtr = 0x14;
			uintptr_t delayPtr = 0x18;
			uintptr_t shotsFired = 0x1C;
			uintptr_t weaponObjPtr = 0xC;
			// offsets relative to weaponObj
			struct {
				uintptr_t recoil = 0x60;
			} weaponObjOFS;
		} currWepOFS;
	} playerentOFS;
} OFS;