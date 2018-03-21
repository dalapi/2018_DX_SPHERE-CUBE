#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "main.h"
#include "Texture.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Uninit();
	void Update();
	void Draw(TEXTURE_ENUM TexNum, D3DXMATRIX mtx);
	void Draw(TEXTURE_ENUM TexNum, D3DXMATRIX mtx, int tcx, int tcy, int tcw, int tch, int tw, int th);
	void SetTex(int tcx, int tcy, int tcw, int tch, int tw, int th);

	typedef struct
	{
		D3DXVECTOR3 pos; // rhwÇäOÇµÇΩ
		D3DXVECTOR3 normal; // ñ@ê¸
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	}VERTEX_3D;

	D3DXMATRIX mtxWorld; // çsóÒ

	static Sprite* Inst()
	{
		if (instance == nullptr) { instance = new Sprite; }
		return instance;
	}
private:
	static Sprite* instance;

	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer;

	static VERTEX_3D vSprite[4];
	static VERTEX_3D* pSprite;
	static WORD Index[6];

};

#endif _SPRITE_H_