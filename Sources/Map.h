#ifndef _MAP_H_
#define _MAP_H_

#include "BlockStage.h"
#include "Texture.h"

class Map
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	static Map* Inst() { if (instance == nullptr) { instance = new Map; } return instance; }
private:
	static Map* instance;
	void Block(TEXTURE_ENUM tex,float x,float y);

	D3DXVECTOR2 BlockPos[MAX_BLOCK];
	TEXTURE_ENUM BlockTex[MAX_BLOCK];

};

#endif