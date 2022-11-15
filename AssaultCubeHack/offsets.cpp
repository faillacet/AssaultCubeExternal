// Enabled Padding of offsets within classes
#define STR_MERGE_IMPL(a,b) a##b
#define STR_MERGE(a,b) STR_MERGE_IMPL(a,b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}
//DEFINE_MEMBER_N: TYPE	 NAME	OFFSET

struct Vector3 { float x, y, z; };

//class Weapon {
//public:
//	union {
//		DEFINE_MEMBER_N(int*, ammoptr, 0x14);
//	};
//};

class Entity {
public:
	union {
		DEFINE_MEMBER_N(Vector3, pos, 0x28);
		DEFINE_MEMBER_N(Vector3, viewAngle, 0x34);
		DEFINE_MEMBER_N(int, health, 0xEC);
		DEFINE_MEMBER_N(int, arAmmo, 0x140);
	};
};



struct {
	int xPos = 0x28;
	int yPos = 0x2C;
	int zPos = 0x30;
	int yaw = 0x34;
	int pitch = 0x38;
	int roll = 0x3C;
	int health = 0xEC;
	int arAmmo = 0x140;
} localPlayerOffsets;