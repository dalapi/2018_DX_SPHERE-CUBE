#ifndef _BLOCK_STAGE_H_
#define _BLOCK_STAGE_H_

#include "main.h"
#include "GUI.h"
#include "Conflict.h"
#include "Texture.h"

// ÉuÉçÉbÉNê›íË
#define BLOCK_WIDTH (9)
#define BLOCK_SPACE (4.0f)
#define MAX_BLOCK (150)
#define BLOCK_SIZE (1.8f)
#define BTIME_MAX (20)
#define BTIME_MIN (80)

#define TARGET (90)

class BlockStage :public GUI
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetCreateTime() { nTimeCreate = rand() % (BTIME_MAX - BTIME_MIN) + BTIME_MIN; }
	void GUIUpdate();

	TEXTURE_ENUM GetTex(int i) { return tex[i]; }
	D3DXVECTOR3 GetPos(int i) { return pos[i]; }
	D3DXVECTOR3 GetMin(int i) { return D3DXVECTOR3(pos[i].x - BLOCK_SIZE / 2, pos[i].y - 3.0f, pos[i].z - BLOCK_SIZE / 2); }
	D3DXVECTOR3 GetMax(int i) { return D3DXVECTOR3(pos[i].x + BLOCK_SIZE / 2, pos[i].y + 3.0f, pos[i].z + BLOCK_SIZE / 2); }

	void SetTex(TEXTURE_ENUM color,int n) { tex[n] = color; }

	static BlockStage* Inst(void)
	{
		if (instance == nullptr) { instance = new BlockStage; }
		return instance;
	}

private:
	static BlockStage* instance;
	D3DXVECTOR3 pos[MAX_BLOCK];
	TEXTURE_ENUM tex[MAX_BLOCK];

	int nCountFrame, nTimeCreate;
};

#endif