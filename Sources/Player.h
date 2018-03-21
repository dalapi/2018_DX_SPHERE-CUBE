#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "GUI.h"
#include "Conflict.h"

#define TIME_MAX (20)
#define TIME_MIN (80)

class Player : public GUI
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void GUIUpdate();
	void Dead();

	void SetCreateTime(){ nTimeCreate = rand() % (TIME_MAX - TIME_MIN) + TIME_MIN;}

	D3DXVECTOR3 Get(void) { return pos; }
	bool GetStart(void) { return bStart; }
	bool GetFloor(void) { return bFloor; }
	bool GetEnd(void) { return nLife <= 0 ? true : false; }
	bool GetGoal(void) { return bGoal; }
	int GetLife(void) { return nLife; }
	int GetBNo(void) { return nBlockNo; }
	int  nBlockNo;

	static Player* Inst()
	{
		if (instance == nullptr){ instance = new Player; }
		return instance;
	}

private:
	Player();
	~Player();
	static Player* instance;

	D3DXMATRIX m_mtxWorld;
	D3DXMATRIX m_mtxSon;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 speed;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 front;

	bool bJump, bStart, bFloor, bScore, bGoal;
	float fJumpValue,fGravity;
	int nLife;
	int nCountFrame, nTimeCreate;

	AABB player;

};


#endif _PLAYER_H_