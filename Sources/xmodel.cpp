#include "xmodel.h"

Model* Model::instance = nullptr;

Model::Model()
{
}

Model::~Model()
{
	if (this->index < 0) { return; }
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (ModelData[i].g_pMesh != nullptr)
		{
			ModelData[i].g_pMesh->Release();
			ModelData[i].g_pMesh = nullptr;
		}
		if (ModelData[i].g_pMaterials != nullptr)
		{
			ModelData[i].g_pMaterials->Release();
			ModelData[i].g_pMaterials = nullptr;
		}
	}
}

bool Model::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	HRESULT hr;
	LPD3DXBUFFER pAdjacency = NULL; // �אځi�œK���������ꍇ�j

	for (int i = 0; i < MODEL_MAX; i++)
	{
		hr = D3DXLoadMeshFromX(aModelFilename[i], D3DXMESH_SYSTEMMEM, pDevice, &pAdjacency, &ModelData[i].g_pMaterials,NULL, &ModelData[i].g_nMaterialNum, &ModelData[i].g_pMesh);
		if (FAILED(hr)) { MessageBox(NULL, "�t�@�C�����[�h�ł��܂���B", "error", MB_OK); return false; }

		hr = ModelData[i].g_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, // ���s���郁�b�V���œK���̎��
													(DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);
		if (FAILED(hr)) { MessageBox(NULL, "�œK�����s���܂����B", "error", MB_OK); return false; }
		if (pAdjacency) { pAdjacency->Release(); }

		LPD3DXMESH pCloneMesh = NULL;
		D3DVERTEXELEMENT9 elements[MAXD3DDECLLENGTH + 1];

		ModelData[i].g_pMesh->GetDeclaration(elements);
		hr = ModelData[i].g_pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, elements, pDevice, &pCloneMesh); // �R�s�[���܂���
		
		if (FAILED(hr)) { MessageBox(NULL, "�N���[�����s���܂����B", "error", MB_OK); return false; }

		ModelData[i].g_pMesh->Release();
		ModelData[i].g_pMesh = pCloneMesh;

		LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)ModelData[i].g_pMaterials->GetBufferPointer(); // �}�e���A���������o��

		// Texture �ǂݍ���
		for (DWORD j = 0; j < ModelData[i].g_nMaterialNum; j++)
		{
			g_pModelTex[i] = NULL; // �e�N�X�`��������
			if (pMaterials[i].pTextureFilename != NULL && strlen(pMaterials[i].pTextureFilename) > 0) // �g�p���Ă���e�N�X�`��������Γǂݍ���
			{
				hr = D3DXCreateTextureFromFile(
					pDevice,
					pMaterials[i].pTextureFilename,
					&g_pModelTex[i]); // �A�h���X�̃A�h���X

				if (FAILED(hr)) { MessageBox(NULL, "���f���e�N�X�`���ǂݍ��܂Ȃ�", "error", MB_OK); return false; }
			}
		}
	}
	return true;
}

void Model::Draw(ENUM numModel, D3DXMATRIX mtxWorld, bool light)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	this->ModelNum = numModel;

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)ModelData[ModelNum].g_pMaterials->GetBufferPointer(); // �}�e���A���������o��

	// ���C�e�B���O
	pDevice->SetRenderState(D3DRS_LIGHTING, light);

	// �s��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	g_pMeshMat = new D3DMATERIAL9[ModelData[ModelNum].g_nMaterialNum];		// ���b�V�������m��
	//g_pModelTex = new LPDIRECT3DTEXTURE9[g_nMaterialNum]; // �e�N�X�`�����m��

	for (DWORD i = 0; i < ModelData[ModelNum].g_nMaterialNum; i++)
	{
		g_pMeshMat[i] = pMaterials[i].MatD3D;	// �}�e���A�����Z�b�g

		//pMaterials[i].pTextureFilename = {"Resource/Texture/horoCube.png"};
		//NULL��������e�N�X�`���Ȃ�	//�p�X��ǉ����ăe�N�X�`�����[�h�֐���

		// ���f�����g�̃}�e���A���֐��������Ƃ��ł��܂�
		g_pMeshMat[i].Ambient = g_pMeshMat[i].Diffuse;

		pDevice->SetMaterial(&g_pMeshMat[i]);
		pDevice->SetTexture(0, g_pModelTex[i]);
		ModelData[ModelNum].g_pMesh->DrawSubset(i);

	}

	delete[] g_pMeshMat;
	g_pMeshMat = nullptr;
}

// 
void Model::Uninit(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}
