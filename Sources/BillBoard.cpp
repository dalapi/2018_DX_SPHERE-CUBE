#include "main.h"
#include "BillBoard.h"
#include "Texture.h"
#include "Effect.h"

// FVFの宣言(定数)
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
		g_pVertexBuffer->Lock(0, 0, (void**)&pBill, D3DLOCK_DISCARD); // LockするとGPU止まってます
		memcpy(pBill, &vBill, sizeof(vBill));
		g_pVertexBuffer->Unlock();
	}
	if (g_pIndexBuffer == nullptr)
	{
		pDevice->CreateIndexBuffer(sizeof(WORD) * 12, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
		LPWORD pIndex;
		g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD); // LockするとGPU止まってます
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
			this->speed = front*0.1f; // スピード

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

	// 今のビュー行列&逆行列
	D3DXMATRIX mtxCurrentView;
	D3DXMATRIX mtxViewRotInv;
	pDevice->GetTransform(D3DTS_VIEW, &mtxCurrentView);
	D3DXMatrixInverse(&mtxViewRotInv, NULL, &mtxCurrentView);

	// 平行移動を消す
	mtxViewRotInv._41 = mtxViewRotInv._42 = mtxViewRotInv._43 = 0.0f;

	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixMultiply(&mtxWorld, &mtxViewRotInv, &mtxWorld);

	//pos = { 2, 2, 0 };
	// 注視點減去相機位置 = 一個運動向量

	this->pos += this->speed;

	// 当たり
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

	// ライティング
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 各種行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// FVF（今から使用する頂点情報）の設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	Texture::Inst()->Set(TEX_BULLET);
	


	// 描画（変更・追加）バッファ追加
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//頂点数、プリミティブ数


}