#ifndef _TIMER_H_
#define _TIMER_H_

#include "GUI.h"

#define TIME_LIMIT (40)

class Timer : public GUI
{
public:
	Timer();
	~Timer();

	void Init();
	void Uninit();
	void Update();
	void Draw(int dig, float x, float y);
	void GUIUpdate();

	bool EndFlag() { return bEnd; }

	static Timer* Inst()
	{
		if (instance == nullptr){instance = new Timer;}
		return instance;
	}

private:
	static Timer* instance;
	int time, count, second;
	bool bEnd = 0;
};

#endif