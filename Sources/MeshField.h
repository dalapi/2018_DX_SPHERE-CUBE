#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

#include "main.h"

class MeshField
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos; // rhw���O����
		D3DXVECTOR3 normal; // �@��
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	}VERTEX_3D;

	void Init(int nHeight, int nWidth);
	void Uninit();
	void Update();
	void Draw(D3DXMATRIX mtx);

	static MeshField* Inst() { if (instance == nullptr) { instance = new MeshField; }return instance; }
private:
	static MeshField* instance;

	// ���_�ƃv���~�e�B�u�����v�Z
	int g_nMaxNum;
	int g_nMaxPriNum;

	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
};

#endif // !_MESH_FIELD_H_
#pragma once
