#ifndef _BACK_GROUND_H_
#define _BACK_GROUND_H_

#include "main.h"

class BackGround
{
public:
	BackGround();
	~BackGround();

	void Uninit();
	void Update();
	void Draw();

	static BackGround* Inst() 
	{
		if (instance == nullptr) { instance = new BackGround; }
		return instance;
	}

private:
	static BackGround* instance;
	D3DXMATRIX mtx,trans,scale;

	int nAnimate;
};

#endif 