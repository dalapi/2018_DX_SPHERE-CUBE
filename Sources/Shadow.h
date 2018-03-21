#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "GUI.h"

class Shadow : public GUI
{
public:
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void GUIUpdate(void);

	static Shadow* Inst() {
		if (instance == nullptr) { instance = new Shadow; }
		return instance;
	}
private:
	Shadow(void);
	~Shadow(void);
	static Shadow* instance;
	
	D3DXVECTOR3 pos;
	D3DXMATRIX mtxWorld;
	
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer;

	// ���_�p�\����(���_�v�f�̐錾)
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos; // rhw���O����
		D3DXVECTOR3 normal; // �@��
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	static VERTEX_3D vBill[4];
	static WORD Index[6];
	float fSizeLength;

};

#endif // !_SHADOW_H_
#pragma once