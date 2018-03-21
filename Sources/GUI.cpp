#include "GUI.h"

GUI* GUI::index[MAX_GUI];

GUI::GUI()
{
	for (int i = 0; i < MAX_GUI; i++)
	{
		if (index[i] == nullptr)
		{
			index[i] = this;
			this->idx = i;
			break;
		}
	}
}

GUI::~GUI()
{
	index[this->idx] = nullptr;
}
