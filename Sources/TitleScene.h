#ifndef _TITLE_H_
#define _TITLE_H_

#include "BaseScene.h"
#include "main.h"

class TitleScene : public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:

	int nChoose,nAnimCount,nPattern;
	float fBlockPos;
};

#endif // _TITLE_H_