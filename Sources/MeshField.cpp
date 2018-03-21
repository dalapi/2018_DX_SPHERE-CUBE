#include "main.h"
#include "MeshField.h"
#include "Texture.h"
#include <vector>

// FVF�̐錾(�萔)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)
MeshField* MeshField::instance = nullptr;


void MeshField::Init(int nWidth,int nHeight)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// �����v�Z
	int nMaxNum = (nHeight+1)*(nWidth+1); // MAX NUM
	int nMaxIndexNum = nMaxNum + (nHeight - 1)*(nWidth + 1) + (nHeight - 1) * 2;
		//nHeight*(nWidth+1)*2+(nHeight-1)*2; // MAX INDEX NUM

	g_nMaxNum = nMaxNum;
	g_nMaxPriNum = nHeight*nWidth * 2 + (nHeight - 1) * 4;

	// �o�b�t�@���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nMaxNum, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * nMaxIndexNum, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);

	// ��������Ԃ��m�ۂ���
	WORD* pIndex = 0;

	std::vector<VERTEX_3D> mesh;
	std::vector<WORD> index;

	// ���_�v�Z�p
	int n = 0;
	float posX = 0.0f - ((float)nWidth / 2);
	float posZ = (float)nHeight / 2;
	int u = 0, v = 0;

	// ���_���
	while (n < nMaxNum)
	{
		VERTEX_3D temp;
		
		temp.pos = D3DXVECTOR3(posX, 0.0f, posZ);
		temp.normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		temp.color = D3DCOLOR_RGBA(255, 255, 255, 255);
		temp.texcoord = D3DXVECTOR2((float)u, (float)v);
		
		mesh.push_back(temp);

		u ++;
		posX += 1.0f;
		n++;

		if (n % (nWidth + 1) == 0)
		{
			posX = 0.0f -((float) nWidth / 2);
			posZ -= 1.0f;
			u = 0;
			v ++;
		}
	}

	// �C���f�b�N�X�v�Z�p
	n = 0;
	int base = 0;

	// �C���f�b�N�X���
	while (n < nMaxIndexNum)
	{
		WORD temp;

		temp = base + (nWidth + 1);
		index.push_back(temp);
		n++;
		temp = base;
		index.push_back(temp);
		n++;
		base++;

		if (base % (nWidth + 1) == 0)
		{
			temp = base - 1;
			index.push_back(temp);
			n++;
			temp = base + (nWidth + 1);
			index.push_back(temp);
			n++;
		}
	}


	// Create Vertex
	VERTEX_3D* Mesh;
	LPWORD Index;

	g_pVertexBuffer->Lock(0, 0, (void**)&Mesh, D3DLOCK_DISCARD); // Lock�����GPU�~�܂��Ă܂�

	for (UINT i = 0; i < mesh.size(); i++)
	{
		Mesh[i] = mesh[i];
	}

	g_pVertexBuffer->Unlock();

	g_pIndexBuffer->Lock(0, 0, (void**)&Index, D3DLOCK_DISCARD);

	for (UINT i = 0; i < index.size(); i++)
	{
		Index[i] = index[i];
	}
	g_pIndexBuffer->Unlock();

	mesh.clear();
}

void MeshField::Uninit()
{
	if (g_pVertexBuffer != NULL){
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	if (g_pIndexBuffer != NULL){
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void MeshField::Update()
{

}

void MeshField::Draw(D3DXMATRIX mtx)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat;
	ZeroMemory(&mat, sizeof(mat));
	
	mat.Diffuse.r = 1.0f;
	mat.Diffuse.g = 1.0f;
	mat.Diffuse.b = 1.0f;
	mat.Diffuse.a = 1.0f;
	
	mat.Ambient.r = 0.8f;
	mat.Ambient.g = 0.8f;
	mat.Ambient.b = 0.8f;
	mat.Ambient.a = 1.0f;
	

	// ���C�e�B���O
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �}�e���A���i�`��O�Ɂj
	pDevice->SetMaterial(&mat);
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// FVF�i������g�p���钸�_���j�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	Texture::Inst()->Set(C_PLUS);

	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nMaxNum, 0, g_nMaxPriNum);//���_���A�v���~�e�B�u��

}
