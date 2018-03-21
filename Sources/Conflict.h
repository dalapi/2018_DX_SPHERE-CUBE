#ifndef _CONFLICT_H_
#define _CONFLICT_H_

#include "main.h"

struct AABB {
	D3DXVECTOR3 min;		// �ő�l
	D3DXVECTOR3 max;		// �ŏ��l
	D3DXVECTOR3 actorOldPos;// �O�̍��W�l

	float test;

	AABB() = default;
	~AABB() = default;

	AABB(D3DXVECTOR3 minValue, D3DXVECTOR3 maxValue, D3DXVECTOR3 actorPos) {
		min = minValue;
		max = maxValue;
		actorOldPos = actorPos;
	}

	// �X�V
	void update(const D3DXVECTOR3& actorPosition) {
		D3DXVECTOR3 diff = actorPosition - actorOldPos;
		min += diff;
		max += diff;
		actorOldPos = actorPosition;
	}
};

class Conflict
{
public:
	// �~
	static bool Circle(D3DXVECTOR3 v1, float f1, D3DXVECTOR3 v2, float f2);
	// AABB
	static bool AABBs(const AABB& box1, const AABB& box2);
};

#endif _CONFLICT_H_