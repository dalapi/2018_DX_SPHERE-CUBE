#include "Texture.h"

Texture* Texture::instance = nullptr;

bool Texture::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	HRESULT hr;

	for (int i = 0; i<TEX_MAX; i++)
	{
		// �e�N�X�`���摜�̓ǂݍ���
		hr = D3DXCreateTextureFromFile(
			pDevice,
			g_aTexture[i].fileName,
			&g_pTexture[i]); // �A�h���X�̃A�h���X
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, "�e�N�X�`�����ǂݍ��߂Ȃ�����", "error", MB_OK);
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