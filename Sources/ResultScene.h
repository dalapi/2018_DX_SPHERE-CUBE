#ifndef _RESULT_H_
#define _RESULT_H_

#include "BaseScene.h"

class ResultScene : public BaseScene
{
public:
	ResultScene();
	~ResultScene();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	//
	int nAnimCount, nPattern;
};

#endif