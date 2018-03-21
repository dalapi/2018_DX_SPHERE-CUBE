#ifndef _XMODEL_H_
#define _XMODEL_H_

#include "main.h"
#include "Texture.h"

// モデルファイルリスト
static const char aModelFilename[][256]
{

	{ "Data/model/sphere2.x" },
//	{ "Data/model/cursor.x" },
};

class Model
{
public:
	enum ENUM
	{

		MODEL_SPHERE = 0,
//		MODEL_CURSOR,
		MODEL_MAX,
	};

	struct Data {
		LPD3DXMESH g_pMesh;			// メッシュデータ
		DWORD g_nMaterialNum;			// マテリアルの数
		LPD3DXBUFFER g_pMaterials;
	};

	bool Init();
	void Draw(ENUM numModel, D3DXMATRIX mtxWorld,bool light);
	void Uninit(void);

	Data Get(int i) { return ModelData[i]; }

	int index = 0;

	static Model* Inst(void)
	{
		if (instance == nullptr)
		{
			instance = new Model;
		}
		return instance;
	}

private:
	Model();
	~Model();

	static Model* instance;

	Data ModelData[MODEL_MAX];		// モデル資料
	ENUM ModelNum;	// モデル番号
	
	D3DMATERIAL9* g_pMeshMat = NULL;	// マテリアル情報
	LPDIRECT3DTEXTURE9 g_pModelTex[MODEL_MAX];	// テクスチャ
};

#endif
