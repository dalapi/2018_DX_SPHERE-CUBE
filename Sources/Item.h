#ifndef _ITEM_H_
#define _ITEM_H_

#include "Object.h"
#include "Texture.h"
#include "main.h"

#define MAX_ITEM (10)

class Item : public Object
{
public:
	Item(TEXTURE_ENUM tex, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	~Item();

	D3DXVECTOR3 pos;

	static Item* Get(int i) { return item[i]; }
private:
	static Item* item[MAX_ITEM];
	void Uninit();
	void Update();
	void Draw();

	int index;

	D3DXMATRIX mtx;
	D3DXVECTOR3 speed, size;
	TEXTURE_ENUM tex;
};

#endif // !_ITEM_H_
#pragma once
