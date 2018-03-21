
#include "Sprite.h"

// FVF�̐錾(�萔)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

Sprite* Sprite::instance = nullptr;

Sprite::VERTEX_3D* Sprite::pSprite;

Sprite::VERTEX_3D Sprite::vSprite[] ={
	{ D3DXVECTOR3(-0.5,1.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f) },
	{ D3DXVECTOR3(0.5, 1.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{ D3DXVECTOR3(-0.5,0.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,1.0f) },
	{ D3DXVECTOR3(0.5,0.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1.0f,1.0f) },
};
WORD Sprite::Index[] = {
	0,1,2,
	1,3,2,
};

// �R���X�g���N�^
Sprite::Sprite()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// �f�[�^���o�b�t�@�ɓ]������
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * 12, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, NULL);

	LPWORD pIndex;

	pVertexBuffer->Lock(0, 0, (void**)&pSprite, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�
	memcpy(pSprite, &vSprite, sizeof(vSprite));
	pVertexBuffer->Unlock();

	pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�
	memcpy(pIndex, &Index, sizeof(Index));
	pIndexBuffer->Unlock();
}

// �f�X�g���N�^
Sprite::~Sprite()
{
	// �o�b�t�@���������
	if (pVertexBuffer != NULL) {
		pVertexBuffer->Release();
		pVertexBuffer = NULL;
	}

	if (pIndexBuffer != NULL) {
		pIndexBuffer->Release();
		pIndexBuffer = NULL;
	}

}

void Sprite::Uninit()
{

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Sprite::Update()
{
}

void Sprite::Draw(TEXTURE_ENUM TexNum,D3DXMATRIX mtx)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���O
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e��s��̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	Texture::Inst()->Set(TexNum);

	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//���_���A�v���~�e�B�u��

}

// �e�N�X�`����ݒ肷��
void Sprite::Draw(TEXTURE_ENUM TexNum, D3DXMATRIX mtx, int tcx, int tcy, int tcw, int tch, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���O
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e��s��̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	Texture::Inst()->Set(TexNum);
	SetTex(tcx, tcy, tcw, tch, tw, th);

	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//���_���A�v���~�e�B�u��

}

void Sprite::SetTex(int tcx, int tcy, int tcw, int tch, int tw,int th) // �T�C�X
{
	pVertexBuffer->Lock(0, 0, (void**)&pSprite, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�

	float u0 = (float)tcx / tw; // left top
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw; // right bottom
	float v1 = (float)(tcy + tch) / th;

	pSprite[0].texcoord = D3DXVECTOR2(u0, v0);
	pSprite[1].texcoord = D3DXVECTOR2(u1, v0);
	pSprite[2].texcoord = D3DXVECTOR2(u0, v1);
	pSprite[3].texcoord = D3DXVECTOR2(u1, v1);

	pVertexBuffer->Unlock();
}