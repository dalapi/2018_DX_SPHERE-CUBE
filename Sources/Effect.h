#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "Object.h"
#include "main.h"
#define MAX_NUM (50)

class Effect : public Object
{
public:
	Effect(D3DXVECTOR3 pos);
	~Effect();

	// ����p
	int g_nAnimCount;
	int g_nPattern;

private:
	static Effect* effect[MAX_NUM];
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateVertex(int tcx, int tcy, int tcw, int tch);
					//Texture�w��Ƃ���ƕ��ƍ���

	// 
	static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
	static LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer;


	int index; // ��

	D3DXMATRIX mtxWorld; // �s��

	// �ʒu
	D3DXVECTOR3 pos;

	// ���_�p�\����(���_�v�f�̐錾)
	typedef struct
	{
		D3DXVECTOR3 pos; // rhw���O����
		D3DXVECTOR3 normal; // �@��
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	}VERTEX_3D;

	static VERTEX_3D vBill[4];
	static VERTEX_3D* pBill;
	static WORD Index[6];
};

#endif //_EFFECT_H_
