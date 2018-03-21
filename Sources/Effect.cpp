#include "Effect.h"
#include "Texture.h"

// FVF�̐錾(�萔)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

LPDIRECT3DVERTEXBUFFER9 Effect::g_pVertexBuffer;
LPDIRECT3DINDEXBUFFER9  Effect::g_pIndexBuffer;

Effect* Effect::effect[MAX_NUM];

Effect::VERTEX_3D Effect::vBill[4];
WORD Effect::Index[6];

Effect::VERTEX_3D* Effect::pBill;

// �G�t�F�N�g�p
Effect::Effect(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	//	HRESULT hr;

	VERTEX_3D vBill[] =
	{
		{ D3DXVECTOR3(-0.5,1.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,0.0f) },
		{ D3DXVECTOR3(0.5, 1.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },
		{ D3DXVECTOR3(-0.5,0.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(0.0f,1.0f) },
		{ D3DXVECTOR3(0.5,0.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) ,D3DXVECTOR2(1.0f,1.0f) },
	};

	WORD Index[] = {
		0,1,2,
		1,3,2,
	};

	if (g_pVertexBuffer == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
		//pBill
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

	for (int i = 0; i < MAX_NUM; i++)
	{
		if (effect[i] == nullptr)
		{
			effect[i] = this;
			this->index = i;
			this->pos = pos;

			break;
		}
	}

	g_nAnimCount = g_nPattern = 0;
}

Effect::~Effect()
{
	if (Effect::effect[this->index] != nullptr)
	{
		Effect::effect[this->index] = nullptr;
	}
}

void Effect::Uninit(void)
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

void Effect::Update()
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

	if (g_nAnimCount < 96) // EXPLOSION_PATTERN_MAX
	{g_nAnimCount++;}
	else{
		g_nAnimCount = 0;
		//������
		this->Destroy();
	}

	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 1.5, 1.5, 1);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	mtxWorld *= scale*trans;
}

void Effect::Draw()
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
	Texture::Inst()->Set(TEX_EXPLOSION);
	g_nPattern = g_nAnimCount / 16; // EXPLOSION_SPEED
	CreateVertex(200 * (g_nPattern % 4), 219 * (g_nPattern /4), 200, 219);


	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//���_���A�v���~�e�B�u��

}

void Effect::CreateVertex(int tcx, int tcy, int tcw, int tch)
{
 	g_pVertexBuffer->Lock(0, 0, (void**)&pBill, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�

	int tw = 800; // g_aTexture[g_TextureNum].width;
	int th = 439; // g_aTexture[g_TextureNum].height;

	float u0 = (float)tcx / tw; // left top
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw; // right bottom
	float v1 = (float)(tcy + tch) / th;

	pBill[0].texcoord = D3DXVECTOR2(u0, v0);
	pBill[1].texcoord = D3DXVECTOR2(u1, v0);
	pBill[2].texcoord = D3DXVECTOR2(u0, v1);
	pBill[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();
}