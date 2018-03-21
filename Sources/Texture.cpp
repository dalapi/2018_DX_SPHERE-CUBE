#include "Texture.h"

Texture* Texture::instance = nullptr;

bool Texture::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	HRESULT hr;

	for (int i = 0; i<TEX_MAX; i++)
	{
		// テクスチャ画像の読み込み
		hr = D3DXCreateTextureFromFile(
			pDevice,
			g_aTexture[i].fileName,
			&g_pTexture[i]); // アドレスのアドレス
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, "テクスチャが読み込めなかった", "error", MB_OK);
		return false;
	}
}

void Texture::Uninit()
{
	for (int i = 0; i < TEX_MAX; i++)
	{
		if (g_pTexture[i] != NULL)
		{
			//g_pTexture[i]->Release();
			g_pTexture[i] = NULL;
		}
	}
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Texture::Set(TEXTURE_ENUM TexNum)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetTexture(0, g_pTexture[TexNum]);
	g_TextureNum = TexNum;
}