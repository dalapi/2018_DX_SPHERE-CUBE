#include "Cube.h"

// FVF�̐錾(�萔)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

// �V���O���g��
Cube* Cube::instance = nullptr;

WORD Cube::Index[] = {
	0,1,2,
	1,3,2,
	4,5,6,
	6,5,7,

	8,9,10,
	10,9,11,
	12,13,14,
	14,13,15,

	16,17,18,
	18,17,19,

	20,21,22,
	22,21,23
};

Cube::VERTEX_3D Cube::vCube[] = {
	// front
	{ D3DXVECTOR3(-0.5, 0.5,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },
	{ D3DXVECTOR3(0.5, 0.5,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{ D3DXVECTOR3(-0.5,-0.5,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },
	{ D3DXVECTOR3(0.5,-0.5,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) },
	// right
	{ D3DXVECTOR3(0.5, 0.5,-0.5),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },
	{ D3DXVECTOR3(0.5, 0.5, 0.5),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{ D3DXVECTOR3(0.5,-0.5,-0.5),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },
	{ D3DXVECTOR3(0.5,-0.5, 0.5),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) },
	// back
	{ D3DXVECTOR3(0.5, 0.5, 0.5) ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },
	{ D3DXVECTOR3(-0.5, 0.5, 0.5),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{ D3DXVECTOR3(0.5,-0.5, 0.5) ,D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },
	{ D3DXVECTOR3(-0.5,-0.5, 0.5),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) },
	// left green
	{ D3DXVECTOR3(-0.5, 0.5, 0.5),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(155,255,255,255),D3DXVECTOR2(0.0f,0.0f) },
	{ D3DXVECTOR3(-0.5, 0.5,-0.5),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(155,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{ D3DXVECTOR3(-0.5,-0.5, 0.5),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(155,255,255,255),D3DXVECTOR2(0.0f,1.0f) },
	{ D3DXVECTOR3(-0.5,-0.5,-0.5),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(155,255,255,255),D3DXVECTOR2(1.0f,1.0f) },
	// up
	{ D3DXVECTOR3( 0.5, 0.5,-0.5),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },//16
	{ D3DXVECTOR3(-0.5, 0.5,-0.5),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },//17
	{ D3DXVECTOR3( 0.5, 0.5, 0.5),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },//18
	{ D3DXVECTOR3(-0.5, 0.5, 0.5),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) },//19
	// buttom black
	{ D3DXVECTOR3( 0.5,-0.5, 0.5),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },//20
	{ D3DXVECTOR3(-0.5,-0.5, 0.5),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },//21
	{ D3DXVECTOR3( 0.5,-0.5,-0.5),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) },//22
	{ D3DXVECTOR3(-0.5,-0.5,-0.5),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) },//23
};

Cube::Cube()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// �w��F
	//for (int i = 0; i < 24; i++) { vCube[i].color = cubeColor; }

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 24, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * 36, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, NULL);

	VERTEX_3D* pV;
	LPWORD pIndex;

	pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�
	memcpy(pV, &vCube, sizeof(vCube)); // �f�[�^��ǂݍ���
	pVertexBuffer->Unlock();

	pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�
	memcpy(pIndex, &Index, sizeof(Index));
	pIndexBuffer->Unlock();
}


Cube::~Cube()
{
	if (pVertexBuffer != NULL){
		pVertexBuffer->Release();
		pVertexBuffer = NULL;
	}

	if (pIndexBuffer != NULL){
		pIndexBuffer->Release();
		pIndexBuffer = NULL;
	}
}

void Cube::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Cube::Update() 
{
}

void Cube::Draw(TEXTURE_ENUM TexNum,D3DXMATRIX mtx)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �e��s��̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	Texture::Inst()->Set(TexNum);

	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);//���_���A�v���~�e�B�u��
}

void Cube::Draw(TEXTURE_ENUM TexNum, D3DXVECTOR3 trans, D3DXVECTOR3 scale)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �s�񐳋K��
	D3DXMATRIX mtx, m_scale, m_trans;
	D3DXMatrixIdentity(&mtx);

	// �ʒu�ݒ肷��
	D3DXMatrixScaling(&m_scale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&m_trans, trans.x, trans.y, trans.z);
	mtx *= m_scale*m_trans;

	// �e��s��̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	Texture::Inst()->Set(TexNum);

	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);//���_���A�v���~�e�B�u��
}