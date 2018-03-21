#include "Object.h"

Object* Object::index[MAX_OBJECT];

Object::Object(void)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (index[i] == nullptr)
		{
			index[i] = this;
			this->idx = i;
			break;
		}
	}
}

Object::~Object(void)
{
	index[this->idx] = nullptr;
}

void Object::UninitObject(void)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (index[i] != nullptr)
		{
			index[i]->Uninit();
			delete index[i];
			index[i] = nullptr;
		}
	}
}

void Object::UpdateObject(void)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (index[i] != nullptr) 
		{
			index[i]->Update();
		}
	}
	DestroyCheck();
}

void Object::DrawObject(void)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (index[i] != nullptr)
		{
			index[i]->Draw();
		}
	}
}

void Object::DestroyCheck(void)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (index[i] != nullptr && index[i]->destroy)
		{
			delete index[i];
			index[i] = nullptr;
		}
	}
}
