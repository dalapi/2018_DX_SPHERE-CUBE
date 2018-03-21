#ifndef _BULLET_H_
#define _BULLET_H_

#include "Object.h"

#define MAX_BULLET (50)
#define BULLET_SIZE (1.0f)

class Bullet : public Object
{
public:
	Bullet(D3DXVECTOR3 pos, D3DXVECTOR3 front);
	~Bullet(void);

	static Bullet* Get(int i) { return bullet[i]; }

	D3DXVECTOR3 pos;

private:
	static Bullet* bullet[MAX_BULLET];
	void Uninit(void);
	void Update(void);
	void Draw(void);


	int index; // ��

	D3DXMATRIX mtxWorld; // �s��

	// �e�p�ʒu�ƕ���
	D3DXVECTOR3 speed;

};

#endif // !_BULLET_H_
#pragma once
