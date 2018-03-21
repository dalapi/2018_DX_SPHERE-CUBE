////////////////////////////////////////////////////////////////////////
//
//	ポリゴン描画
//												 2017.06.05 Who wrote it
////////////////////////////////////////////////////////////////////////
#include "polygon.h"

//テクスチャバッファ
static LPDIRECT3DTEXTURE9 g_pTex[TEX_MAX];
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;

static float g_fPolygonScaleX = 1.0f;
static float g_fPolygonScaleY = 1.0f;
static D3DCOLOR g_PolygonColor = 0xffffffff;
static TEXTURE_ENUM g_TextureNum = TEX_MAX;

static void CreateVertex(float dx, float dy, int tcx, int tcy, int tcw, int tch);
static void CreateVertexAffine(float dx, float dy, int tcx, int tcy, int tcw, int tch);

static float g_fScaleX = 1.0f;
static float g_fScaleY = 1.0f;
static float g_fScaleOffsetX = 1.0f;
static float g_fScaleOffsetY = 1.0f;

static float g_fScale = 1.0f;;
static float g_fAngle = 0.0f;
static float g_fRotaionOffsetX = 0.0f;
static float g_fRotaionOffsetY = 0.0f;


// 構造体
// 頂点用構造体
typedef struct
{
	D3DXVECTOR4 pos;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord; // texture
}VERTEX_2D;

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

bool InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) {return false;}

	for (int i = 0; i < TEX_MAX; i++)
	{
		g_pTex[i] = Texture::Inst()->GetTex(i);
	}

	HRESULT hr;

	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4, // 頂点一個分かける四
		D3DUSAGE_WRITEONLY, // 読むオンリー（卒業前変わらないかも）
		FVF_VERTEX_2D, // 実はゼロでもおっけ
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
		);

	if (FAILED(hr)) // 	{
	{
		MessageBox(NULL, "2Dポリゴンバッファ生成失敗しました。", "error", MB_OK);
		return false;
	}

	return true;
}

void UninitPolygon(void)
{
	for (int i = 0; i < TEX_MAX; i++)
	{
		if (g_pTex[i] != NULL)
		{
			g_pTex[i]->Release();
			g_pTex[i] = NULL;
		}
	}

	if (g_pVertexBuffer != NULL)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void UpdatePolygon(void)
{
}

void SetPolygonColor(D3DCOLOR color)
{
	g_PolygonColor = color;
}

void SetPolygonScale(float sx, float sy)
{
	g_fPolygonScaleX = sx;
	g_fPolygonScaleY = sy;
}


void SetPolygonRotation(float cx, float cy, float angle)
{
	g_fRotaionOffsetX = cx;
	g_fRotaionOffsetY = cy;
	g_fAngle = angle;
}

void Set2DTexture(TEXTURE_ENUM TexNum)
{
	g_TextureNum = TexNum;
}

void DrawPolygon(float dx, float dy,
				 int tcx, int tcy, int tcw, int tch, bool bAffine = false )
	{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return ; }

	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	if (bAffine) // もし角度指定すると
	{
		CreateVertexAffine(dx, dy, tcx, tcy, tcw, tch);
	}
	else
	{
		CreateVertex(dx, dy, tcx, tcy, tcw, tch);
	}

	// バッファ追加
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_2D));

	// FVF（今から使用する頂点情報）の設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, g_pTex[g_TextureNum]);


	// 描画 （バッファ用）
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,0, 2);


	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CreateVertex(float dx, float dy, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;

	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD); // LockするとGPU止まってます

	pV[0].pos = D3DXVECTOR4(-0.5f + dx + 0.0f * g_fPolygonScaleX, -0.5f + dy + 0.0f * g_fPolygonScaleY, 1.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(-0.5f + dx + tcw * g_fPolygonScaleX, -0.5f + dy + 0.0f * g_fPolygonScaleY, 1.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(-0.5f + dx + 0.0f * g_fPolygonScaleX, -0.5f + dy + tch * g_fPolygonScaleY, 1.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(-0.5f + dx + tcw * g_fPolygonScaleX, -0.5f + dy + tch * g_fPolygonScaleY, 1.0f, 1.0f);

	pV[0].color =
	pV[1].color =
	pV[2].color =
	pV[3].color = g_PolygonColor; //hint:これはいける

	int tw = g_aTexture[g_TextureNum].width;
	int th = g_aTexture[g_TextureNum].height;

	float u0 = (float)tcx / tw; // left top
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw; // right bottom
	float v1 = (float)(tcy + tch) / th;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();

}

void CreateVertexAffine(float dx, float dy, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;

	g_pVertexBuffer->Lock(0, 0,(void**)&pV, D3DLOCK_DISCARD); // LockするとGPU止まってます

	float fRotationOffsetX = g_fRotaionOffsetX * g_fScaleX;
	float fRotationOffsetY = g_fRotaionOffsetY * g_fScaleY;

	float x = (0.0f - g_fScaleOffsetX) * g_fScaleX + (g_fScaleOffsetX * g_fScaleX);
	float x_w = (tcw - g_fScaleOffsetX) * g_fScaleX + (g_fScaleOffsetX * g_fScaleX);
	float y = (0.0f - g_fScaleOffsetY) * g_fScaleY + (g_fScaleOffsetY * g_fScaleY);
	float y_h = (tch - g_fScaleOffsetY) * g_fScaleY + (g_fScaleOffsetY * g_fScaleY);

	x -= g_fRotaionOffsetX;
	x_w -= g_fRotaionOffsetX;
	y -= g_fRotaionOffsetY;
	y_h -= g_fRotaionOffsetY;
	dx += g_fRotaionOffsetX;
	dy += g_fRotaionOffsetY;

	pV[0].pos = D3DXVECTOR4(x   * cosf(g_fAngle) - y   *sinf(g_fAngle) + dx, x   *sinf(g_fAngle) + y  *cosf(g_fAngle) + dy, 1.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(x_w * cosf(g_fAngle) - y   *sinf(g_fAngle) + dx, x_w *sinf(g_fAngle) + y  *cosf(g_fAngle) + dy, 1.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(x   * cosf(g_fAngle) - y_h *sinf(g_fAngle) + dx, x   *sinf(g_fAngle) + y_h*cosf(g_fAngle) + dy, 1.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(x_w * cosf(g_fAngle) - y_h *sinf(g_fAngle) + dx, x_w *sinf(g_fAngle) + y_h*cosf(g_fAngle) + dy, 1.0f, 1.0f);

	pV[0].color =
	pV[1].color =
	pV[2].color =
	pV[3].color = g_PolygonColor; //hint:これはいける

	int tw = g_aTexture[g_TextureNum].width;
	int th = g_aTexture[g_TextureNum].height;

	float u0 = (float)tcx / tw; // left top
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw; // right bottom
	float v1 = (float)(tcy + tch) / th;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();

}
