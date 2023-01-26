#pragma once

struct {

	unsigned int recoilFunc = 0xC8BA0;
	unsigned int playerent = 0x18AC00;
	// offsets relative to playerent
	struct {
		unsigned int headPosX = 0x4;
		unsigned int headPosY = 0x8;
		unsigned int headPosZ = 0xC;
		unsigned int posX = 0x28;
		unsigned int posY = 0x2C;
		unsigned int posZ = 0x30;
		unsigned int viewX = 0x34;
		unsigned int viewY = 0x38;
		unsigned int viewZ = 0x3C;
		unsigned int health = 0xEC;
		unsigned int armor = 0xF0;

		unsigned int currWepPtr = 0x364;
		// offsets relative to currWepPtr
		struct {
			unsigned int id = 0x4;
			unsigned int ammoReservePtr = 0x10;
			unsigned int ammoPtr = 0x14;
			unsigned int delayPtr = 0x18;
			unsigned int shotsFired = 0x1C;
			unsigned int weaponObjPtr = 0xC;
			// offsets relative to weaponObj
			struct {
				unsigned int recoil = 0x60;
			} weaponObjOFS;
		} currWepOFS;
	} playerentOFS;
} OFS;