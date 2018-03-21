#include "Item.h"
#include "Cube.h"
#include "Player.h"
#include "Conflict.h"
#include "Score.h"
#include "sound.h"
#include "Effects.h"

Item* Item::item[MAX_ITEM];

Item::Item(TEXTURE_ENUM tex, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (item[i] == nullptr)
		{
			item[i] = this;
			this->index = i;

			this->pos = pos;
			this->tex = tex;
			this->size = size;

			break;
		}
	}
}

Item::~Item()
{
	if (this->index < 0) { return; }
	if (Item::item[this->index] != nullptr){
		Item::item[this->index] = nullptr;}
}

void Item::Uninit()
{
}

void Item::Update()
{
	// ³‹K‰»
	D3DXMatrixIdentity(&mtx);

	//this->pos += this->speed;


	// Player‚Æ‚Ì“–‚½‚è
	if (Conflict::Circle(this->pos, 1.0f, Player::Inst()->Get(), 1.0f))
	{
		this->Destroy();
		Score::Inst()->Plus(20 + rand() % 20);
		Sound::Inst()->Play(SE_KAMI);
		Sound::Inst()->Play(SE_GET);
		Effects::Inst()->Play(DROP, Player::Inst()->Get());
	}

	// Šg‘åk¬
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, size.x, size.y, size.z);

	// ˆÚ“®
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	mtx *= scale*trans;
}

void Item::Draw()
{
	Cube::Inst()->Draw(tex, mtx);
}