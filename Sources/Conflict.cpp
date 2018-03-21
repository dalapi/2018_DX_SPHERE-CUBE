#include "Conflict.h"

// â~sÇÃè’ìÀîªíË
bool Conflict::Circle(D3DXVECTOR3 v1, float f1, D3DXVECTOR3 v2, float f2)
{
	// (x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2 <= (r1+r2)^2
	float compare = powf(v2.x-v1.x,2.0)+powf(v2.y-v1.y,2.0)+powf(v2.z-v1.z,2.0);
	return (compare <= powf(f1 + f2, 2.0)) ? true : false;
}

// AABBsÇÃè’ìÀîªíË
bool Conflict::AABBs(const AABB& box1, const AABB& box2) {
	// ç≈ëÂÇ∆ç≈è¨
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;
	return true;   // è’ìÀ
}

