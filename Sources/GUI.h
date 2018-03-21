#ifndef _GUI_H_
#define _GUI_H_

#include "ImGUI\imgui.h"

#define MAX_GUI (150)

class GUI
{
public:
	GUI(void);
	~GUI(void);

	static GUI* Get(int i) { return index[i]; }

	virtual void GUIUpdate() {};

private:
	static GUI* index[MAX_GUI];

	int idx;
};

#endif