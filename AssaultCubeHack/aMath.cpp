#include "aMath.h"

float aMath::getDistance(Vec3 src, Vec3 other) {
	Vec3 delta = Vec3(other.x - src.x, other.y - src.y, other.z - src.z);
	return sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

float aMath::magnitude(Vec3 src) {
	return sqrtf(src.x * src.x + src.y * src.y + src.z * src.z);
}

float aMath::distance(Vec3 src, Vec3 dst) {
	Vec3 diff = vecSub(src, dst);
	return magnitude(diff);
}

Vec3 aMath::vecSub(Vec3 src, Vec3 dst) {
	Vec3 diff(src.x - dst.x, src.y - dst.y, src.z - dst.z);
	return diff;
}

Vec3 aMath::calcAngle(Vec3 src, Vec3 dst) {
	Vec3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / PI * 180.0f + 180.0f;
	angle.y = asinf((dst.z - src.z) / distance(src, dst)) * 180.0f / PI;
	angle.z = 0.0f;
	return angle;
}