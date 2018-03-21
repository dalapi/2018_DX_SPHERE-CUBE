#ifndef _SCORE_H_
#define _SCORE_H_

#include "GUI.h"

class Score : public GUI
{
public:
	Score();
	~Score();

	void Init();
	void Uninit();
	void Plus(int Plus);
	void Draw(int d, float x, float y);
	int Get();
	void GUIUpdate();

	static Score* Inst()
	{
		if (instance == nullptr){instance = new Score;}
		return instance;
	}

private:
	static Score* instance;

	int nScore;

};

#endif _SCORE_H_
