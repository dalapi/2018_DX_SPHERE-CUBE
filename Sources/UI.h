#ifndef _UI_H_
#define _UI_H_

#include "main.h"

class UI
{
public:
	UI();
	~UI();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static UI* Inst() {
		if (instance == nullptr) { instance = new UI; }
		return instance;
	}

private:
	static UI* instance;

	bool bStart; // ゲームスタートフラグ
	D3DXMATRIX mStartLabel;
};

#endif _UI_H_