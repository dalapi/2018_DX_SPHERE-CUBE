#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "BaseScene.h"

class GameScene :public BaseScene
{
public:
	GameScene();
	~GameScene();
	void Init();
	void Uninit();
	void Update();
	void Draw(); 

	void EndFlag() { bEnd = 1; }
	void EndUpdate();
	void EndDraw();

	void PauseUpdate();
	void PauseDraw();
private:
	//�Q�[���V�[���ŕK�v�Ȃ���
	bool bPause = false;	// �|�[�X�L�[
	bool bStart = false;	// �X�^�[�g�L�[
	bool bEnd, bGoal;

	int nAnimCount, nPattern;
};

#endif // _GAMESCENE_H_
