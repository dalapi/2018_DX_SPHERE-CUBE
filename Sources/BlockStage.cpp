#include "BlockStage.h"
#include "Cube.h"
#include "Effects.h"


BlockStage* BlockStage::instance = nullptr;

void BlockStage::Init()
{
	int n = 0;
	int base = 0;
	float posX = 0.0f;
	float posZ = 0.0f;

	float posY = (rand() % 3)*0.9f;

	while (n < MAX_BLOCK)
	{
		tex[n] = C_WHITE;
		pos[n] = D3DXVECTOR3((-(BLOCK_WIDTH-1)*BLOCK_SPACE/2.0f) + (posX*BLOCK_SPACE), -6.0f+posY, -20.0f + (posZ*BLOCK_SPACE));
		n++;

		posX += 1.0f;
		posY = (rand() % 4)*0.9f;

		if (n % BLOCK_WIDTH == 0)
		{
			posZ += 1.0f;
			posX = 0.0f;
		}

	}

	// ターゲット
	tex[TARGET] = C_BLUE;

	// 最初のランダム値
	nCountFrame = nTimeCreate = 0;
	SetCreateTime();
}

void BlockStage::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void BlockStage::Update()
{
	nCountFrame++;

	if (nCountFrame == nTimeCreate)
	{
		Effects::Inst()->Play(GATE, D3DXVECTOR3(pos[TARGET].x+rand()%7, pos[TARGET].y+3.0f, pos[TARGET].z + rand() % 7));
		Effects::Inst()->Play(GATE, D3DXVECTOR3(pos[TARGET].x + rand() % 7, pos[TARGET].y + 3.0f, pos[TARGET].z + rand() % 7));
		Effects::Inst()->Play(GATE, D3DXVECTOR3(pos[TARGET].x -10.0f- rand() % 7, pos[TARGET].y + 5.0f, pos[TARGET].z + rand() % 7));
		Effects::Inst()->Play(GATE, D3DXVECTOR3(pos[TARGET].x -10.0f- rand() % 7, pos[TARGET].y + 5.0f, pos[TARGET].z + rand() % 7));

		nCountFrame = 0;
		SetCreateTime();
	}
	Effects::Inst()->Update();
}

void BlockStage::Draw()
{
	// building(TEX,TRANS,SCALE)
	for (int n = 0; n < MAX_BLOCK; n++)
	{
		Cube::Inst()->Draw(tex[n], pos[n], D3DXVECTOR3(2.0f, 5.0f, 2.0f));
		Cube::Inst()->Draw(C_WHITE, D3DXVECTOR3(pos[n].x, 10.0f, pos[n].z), D3DXVECTOR3(2.0f, 5.0f, 2.0f));

	}
}

void BlockStage::GUIUpdate()
{
	ImGui::Text("BlockStage");
	ImGui::Text("pos[90] %.2f y %.2f z %.2f", pos[TARGET].x, pos[TARGET].y, pos[TARGET].z);
}