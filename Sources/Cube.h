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

	// ���_�p�\����
	typedef struct {
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	}VERTEX_3D;


	D3DXMATRIX mtxWorld; // �s��

	// �V���O���g��
	static Cube* Inst() {
		if (instance == nullptr){instance = new Cube;}
		return instance;}

private:
	// ����
	static Cube* instance;

	// �o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;

	// �L���[�u
	static VERTEX_3D vCube[24];
	static WORD Index[36];
};

#endif _CUBE_H_
