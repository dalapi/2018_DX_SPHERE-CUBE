#include "BackGround.h"
#include "polygon.h"

BackGround* BackGround::instance = nullptr;

BackGround::BackGround()
{
	nAnimate = 0;
}


BackGround::~BackGround()
{

}

void BackGround::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void BackGround::Update()
{
	if (nAnimate < 800) { nAnimate++; }
	else { nAnimate = 0; }
	
}

void BackGround::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); // 解析度が変わる
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); // 拡大縮小の場合は
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); // 線が綺麗でなります

	// 背景
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(C_BLACK);
	DrawPolygon(0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	// 背景
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 155));
	Set2DTexture(TEX_KUMO);
	DrawPolygon(0, 0, nAnimate, 0, 1024, 768, false);
}
