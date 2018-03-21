#include "Map.h"
#include "Polygon.h"
#include "Player.h"

Map* Map::instance = nullptr;

void Map::Init()
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		BlockPos[i].x = BlockStage::Inst()->GetPos(i).x;
		BlockPos[i].y = BlockStage::Inst()->GetPos(i).z;
		BlockTex[i] = BlockStage::Inst()->GetTex(i);
	}
}

void Map::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Map::Update()
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		BlockTex[i] = BlockStage::Inst()->GetTex(i);
	}

	BlockTex[Player::Inst()->GetBNo()] = C_BLUE;
}

void Map::Draw()
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block(BlockTex[i],BlockPos[i].x, BlockPos[i].y);
	}
}

void Map::Block(TEXTURE_ENUM tex, float x, float y)
{
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(tex);
	DrawPolygon(x, y, 0, 0, 10, 10, false);
}