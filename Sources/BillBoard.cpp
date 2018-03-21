#include "main.h"
#include "BillBoard.h"
#include "Texture.h"
#include "Effect.h"

// FVF�̐錾(�萔)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

LPDIRECT3DVERTEXBUFFER9 BillBoard::g_pVertexBuffer;
LPDIRECT3DINDEXBUFFER9  BillBoard::g_pIndexBuffer;

BillBoard::VERTEX_3D BillBoard::vBill[] =
{
	{ D3DXVECTOR3(-0.5,1.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f) },
	{ D3DXVECTOR3(0.5, 1.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
	{ D3DXVECTOR3(-0.5,0.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,1.0f) },
	{ D3DXVECTOR3(0.5,0.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1.0f,1.0f) },
};

WORD BillBoard::Index[] = {
	0,1,2,
	1,3,2,
};

BillBoard* BillBoard::billboard[MAX_BILLBOARD];

BillBoard::BillBoard(D3DXVECTOR3 pos,D3DXVECTOR3 front)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (g_pVertexBuffer == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
		VERTEX_3D* pBill;
		g_pVertexBuffer->Lock(0, 0, (void**)&pBill, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�
		memcpy(pBill, &vBill, sizeof(vBill));
		g_pVertexBuffer->Unlock();
	}
	if (g_pIndexBuffer == nullptr)
	{
		pDevice->CreateIndexBuffer(sizeof(WORD) * 12, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
		LPWORD pIndex;
		g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�
		memcpy(pIndex, &Index, sizeof(Index));
		g_pIndexBuffer->Unlock();
	}


	for (int i = 0; i < MAX_BILLBOARD; i++)
	{
		if (billboard[i] == nullptr)
		{
			billboard[i] = this;
			this->index = i;
			
			this->pos = pos;
			D3DXVec3Normalize(&front, &front);
			this->speed = front*0.1f; // �X�s�[�h

			break;
		}
	}
}
BillBoard::~BillBoard()
{
	if (this->index < 0) { return; }
	if (BillBoard::billboard[this->index] != nullptr)
	{
		BillBoard::billboard[this->index] = nullptr;
	}
}

void BillBoard::Uninit(void)
{
	if (g_pVertexBuffer != NULL) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	if (g_pIndexBuffer != NULL) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
}

void BillBoard::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// ���̃r���[�s��&�t�s��
	D3DXMATRIX mtxCurrentView;
	D3DXMATRIX mtxViewRotInv;
	pDevice->GetTransform(D3DTS_VIEW, &mtxCurrentView);
	D3DXMatrixInverse(&mtxViewRotInv, NULL, &mtxCurrentView);

	// ���s�ړ�������
	mtxViewRotInv._41 = mtxViewRotInv._42 = mtxViewRotInv._43 = 0.0f;

	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixMultiply(&mtxWorld, &mtxViewRotInv, &mtxWorld);

	//pos = { 2, 2, 0 };
	// �����y�������@�ʒu = ��^������

	this->pos += this->speed;

	// ������
	if (this->pos.x < -9.5f || 9.5f< this->pos.x ||
		this->pos.z < -9.5f || 9.5f< this->pos.z)
	{
		//D3DXVECTOR3 p = this->pos;
		this->Destroy();
		new Effect(this->pos);
	}


	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 0.4f, 0.4f, 1.0f);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	mtxWorld *= scale*trans;

	
}
void BillBoard::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���O
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e��s��̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	Texture::Inst()->Set(TEX_BULLET);
	


	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//���_���A�v���~�e�B�u��


}