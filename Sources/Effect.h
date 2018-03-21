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

	// 動画用
	int g_nAnimCount;
	int g_nPattern;

private:
	static Effect* effect[MAX_NUM];
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateVertex(int tcx, int tcy, int tcw, int tch);
					//Texture指定ところと幅と高さ

	// 
	static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
	static LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer;


	int index; // 個数

	D3DXMATRIX mtxWorld; // 行列

	// 位置
	D3DXVECTOR3 pos;

	// 頂点用構造体(頂点要素の宣言)
	typedef struct
	{
		D3DXVECTOR3 pos; // rhwを外した
		D3DXVECTOR3 normal; // 法線
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	}VERTEX_3D;

	static VERTEX_3D vBill[4];
	static VERTEX_3D* pBill;
	static WORD Index[6];
};

#endif //_EFFECT_H_
