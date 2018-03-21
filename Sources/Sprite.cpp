
#include "Sprite.h"

// FVFの宣言(定数)
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

// コンストラクタ
Sprite::Sprite()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// データをバッファに転送する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * 12, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, NULL);

	LPWORD pIndex;

	pVertexBuffer->Lock(0, 0, (void**)&pSprite, D3DLOCK_DISCARD); // LockするとGPU止まってます
	memcpy(pSprite, &vSprite, sizeof(vSprite));
	pVertexBuffer->Unlock();

	pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD); // LockするとGPU止まってます
	memcpy(pIndex, &Index, sizeof(Index));
	pIndexBuffer->Unlock();
}

// デストラクタ
Sprite::~Sprite()
{
	// バッファを解放する
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

	// ライティング
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 各種行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// FVF（今から使用する頂点情報）の設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	Texture::Inst()->Set(TexNum);

	// 描画（変更・追加）バッファ追加
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//頂点数、プリミティブ数

}

// テクスチャを設定する
void Sprite::Draw(TEXTURE_ENUM TexNum, D3DXMATRIX mtx, int tcx, int tcy, int tcw, int tch, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// ライティング
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 各種行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// FVF（今から使用する頂点情報）の設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	Texture::Inst()->Set(TexNum);
	SetTex(tcx, tcy, tcw, tch, tw, th);

	// 描画（変更・追加）バッファ追加
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);//頂点数、プリミティブ数

}

void Sprite::SetTex(int tcx, int tcy, int tcw, int tch, int tw,int th) // サイス
{
	pVertexBuffer->Lock(0, 0, (void**)&pSprite, D3DLOCK_DISCARD); // LockするとGPU止まってます

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