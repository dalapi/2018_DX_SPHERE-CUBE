#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "Object.h"

#define MAX_BILLBOARD (10)

class BillBoard : public Object
{
public:
	BillBoard(D3DXVECTOR3 pos, D3DXVECTOR3 front);
	~BillBoard(void);

	static BillBoard* Get(int i) { return billboard[i]; }

private: 
	static BillBoard* billboard[MAX_BILLBOARD];
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//Vertex�����\��

	// 
	static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
	static LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer;

	int index; // ��
	
	D3DXMATRIX mtxWorld; // �s��

	// �e�p�ʒu�ƕ���
	D3DXVECTOR3 pos;
	D3DXVECTOR3 speed;

	// ���_�p�\����(���_�v�f�̐錾)
	typedef struct
	{
		D3DXVECTOR3 pos; // rhw���O����
		D3DXVECTOR3 normal; // �@��
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	}VERTEX_3D;

	static VERTEX_3D vBill[4];
	static WORD Index[6];
};

#endif // !_BILLBOARD_H_
#pragma once
