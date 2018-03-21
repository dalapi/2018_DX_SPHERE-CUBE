#ifndef _FIELD_CONTROL_
#define _FIELD_CONTROL_

#include "main.h"

class FieldController
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	static FieldController* Inst() 
	{
		if (instance == nullptr) { instance = new FieldController; }
		return instance;
	}

private:
	FieldController();
	~FieldController();
	static FieldController* instance;

	D3DXMATRIX mtx;

};

#endif