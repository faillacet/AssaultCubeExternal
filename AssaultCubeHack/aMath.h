#pragma once
#include "includes.h"
#define PI 3.1415927f

namespace aMath {
	float getDistance(Vec3 src, Vec3 other);
	float magnitude(Vec3 src);
	float distance(Vec3 src, Vec3 dst);
	Vec3 vecSub(Vec3 src, Vec3 dst);
	Vec3 calcAngle(Vec3 src, Vec3 dst);
}