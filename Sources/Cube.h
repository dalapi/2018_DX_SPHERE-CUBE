#ifndef _CUBE_H_
#define _CUBE_H_

#include "main.h"
#include "Texture.h"

class Cube
{
public:
	Cube();
	~Cube();

	void Uninit();
	void Update();
	void Draw(TEXTURE_ENUM TexNum,D3DXMATRIX mtx);
	void Draw(TEXTURE_ENUM TexNum, D3DXVECTOR3 trans, D3DXVECTOR3 scale);

	// 頂点用構造体
	typedef struct {
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	}VERTEX_3D;


	D3DXMATRIX mtxWorld; // 行列

	// シングルトン
	static Cube* Inst() {
		if (instance == nullptr){instance = new Cube;}
		return instance;}

private:
	// 実体
	static Cube* instance;

	// バッファ
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;

	// キューブ
	static VERTEX_3D vCube[24];
	static WORD Index[36];
};

#endif _CUBE_H_
