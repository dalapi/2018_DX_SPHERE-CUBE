#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Texture.h"
#include "Object.h"
#include "main.h"
#include "GUI.h"

#define MAX_ENEMY (100) // “¯•\¦‚Å‚«‚é“G”

#define CREATE_TIME_MIN  (20)	// “G¶¬Å¬ŠÔ
#define CREATE_TIME_MAX (100)	// “K¶¬Å‘åŠÔ

enum ENEMY_TYPE {
	FLY,
	JUMP,
};

class Enemy : public Object, GUI
{
public:
	Enemy(TEXTURE_ENUM tex, D3DXVECTOR3 pos, D3DXVECTOR3 front, D3DXVECTOR3 size, ENEMY_TYPE type);
	~Enemy();

	D3DXVECTOR3 pos;
	static Enemy* Get(int i) { return enemy[i]; }
private:

	static Enemy* enemy[MAX_ENEMY];
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int index;

	D3DXMATRIX mtx;
	D3DXVECTOR3 speed,size;
	TEXTURE_ENUM tex;
	ENEMY_TYPE type;


};

class CreateEnemy : public GUI
{
public:
	CreateEnemy();
	~CreateEnemy();

	void Uninit();
	void Update();
	void SetCreateTime(void);
	void GUIUpdate(void);

	static CreateEnemy* Inst()
	{
		if (instance == nullptr) { instance = new CreateEnemy; }
		return instance;
	}
private:
	static CreateEnemy* instance;

	int nCountFrame;
	int nTimeCreate;
};

#endif