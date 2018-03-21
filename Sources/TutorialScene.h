#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "BaseScene.h"
#include "main.h"

class TutorialScene : public BaseScene
{
public:
	TutorialScene();
	~TutorialScene();
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	D3DXMATRIX mtx;
	D3DXVECTOR3 rot;

	int nChoose, nAnimCount, nPattern;
	float fBlockPos;
};


#endif // !_TUTORIAL_H_


