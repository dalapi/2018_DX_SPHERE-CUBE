#include "Shadow.h"
#include "Texture.h"
#include "Player.h"
#include "BlockStage.h"

// FVFの宣言(定数)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

Shadow* Shadow::instance = nullptr;

Shadow::VERTEX_3D Shadow::vBill[] =
{
	{ D3DXVECTOR3(-0.5,1.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255, 155) ,D3DXVECTOR2(0.0f,0.0f) },
	{ D3DXVECTOR3(0.5, 1.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,155),D3DXVECTOR2(1.0f,0.0f) },
	{ D3DXVECTOR3(-0.5,0.0,-0.5),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255, 155) ,D3DXVECTOR2(0.0f,1.0f) },
	{ D3DXVECTOR3(0.5,0.0,-0.5) ,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255, 155) ,D3DXVECTOR2(1.0f,1.0f) },
};

WORD Shadow::Index[] = {
	0,1,2,
	1,3,2,
};

Shadow::Shadow()
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

	pos = { 0,0,0 };
	fSizeLength = 0.0f;
}


Shadow::~Shadow()
{
	if (g_pVertexBuffer != nullptr) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = nullptr;
	}

	if (g_pIndexBuffer != nullptr) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = nullptr;
	}
}

void Shadow::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Shadow::Update()
{
	// 高さより大きさ変更する
	pos = Player::Inst()->Get();
	if(Player::Inst()->GetFloor()){ pos.y = BlockStage::Inst()->GetPos(Player::Inst()->nBlockNo).y + 2.5f; }
	else { pos.y = -5.0f; }
	fSizeLength = 1 /fabs(Player::Inst()->Get().y - BlockStage::Inst()->GetPos(Player::Inst()->nBlockNo).y + 2.5);

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	D3DXMatrixIdentity(&mtxWorld);
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, fSizeLength*1.5, 1.0f, fSizeLength*1.5);
	D3DXMATRIX rot;
	D3DXMatrixRotationX(&rot, 1.62f);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	mtxWorld *= rot*scale*trans;
}

void Shadow::Draw()
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
	Texture::Inst()->Set(TEX_SHADOW);

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//頂点数、プリミティブ数

	// 戻る
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//αカラーの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void Shadow::GUIUpdate()
{
	//ImGui::Text("Shadow");
	//ImGui::Text("PlayerY %2f", Player::Inst()->Get().y);
	//ImGui::Text("BlockY %2f", BlockStage::Inst()->GetPos(Player::Inst()->nBlockNo).y);
	//ImGui::Text("1/(PY-BY) %2f", fSizeLength);
}