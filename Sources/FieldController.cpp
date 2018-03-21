#include "FieldController.h"
#include "MeshField.h"
#include "BlockStage.h"
#include "Cube.h"
#include "Item.h"

FieldController* FieldController::instance = nullptr;

FieldController::FieldController()
{
	// デバッグ用サイステスト
	// size_t a = sizeof(FieldController);
}

FieldController::~FieldController()
{
}

void FieldController::Init()
{
	MeshField::Inst()->Init(50, 100);
	BlockStage::Inst()->Init();

	// 行列正規化
	D3DXMatrixIdentity(&mtx);
	D3DXMATRIX scale, trans;

	// フィールド
	D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixTranslation(&trans, 0.0f, -5.0f, 0.0f);
	mtx *= scale*trans;

	// アイテム配置
	new Item(C_QUES, D3DXVECTOR3(8, 2.5, 5), D3DXVECTOR3(1, 1, 1));
	new Item(C_QUES, D3DXVECTOR3(-3, 3, 15), D3DXVECTOR3(1, 1, 1));
	new Item(C_QUES, D3DXVECTOR3(-3+rand()%7, 3, 15-rand()%5), D3DXVECTOR3(1, 1, 1));
	new Item(C_PLUS, D3DXVECTOR3(-10 + rand() % 7, 2, 0 + rand() % 5), D3DXVECTOR3(1, 1, 1));
}

void FieldController::Uninit()
{
	MeshField::Inst()->Uninit();
	BlockStage::Inst()->Uninit();
	Cube::Inst()->Uninit();

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void FieldController::Update()
{
	MeshField::Inst()->Update();
	BlockStage::Inst()->Update();
}

void FieldController::Draw()
{
	MeshField::Inst()->Draw(mtx);
	BlockStage::Inst()->Draw();
}