#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

typedef struct {
	char fileName[256];
	int width;
	int height;
}TEXTURE;

static const TEXTURE g_aTexture[] = {
	{ "Data/Texture/oji.jpg", 50,50 },
	{ "Data/Texture/field001.jpg", 50,50 },
	{ "Data/Texture/field002.png", 64,64 },
	{ "Data/Texture/bullet.png", 30,30 },
	{ "Data/Texture/shadow000.jpg", 80,80 },
	{ "Data/Texture/explosion.png", 800,439 },
	{ "Data/Texture/1.jpg", 800,600 },
	{ "Data/Texture/mosaic.jpg", 1600,600 },

	{ "Data/Texture/basic/pink.png", 30,30 },
	{ "Data/Texture/basic/green.png", 30,30 },
	{ "Data/Texture/basic/blue.png", 30,30 },
	{ "Data/Texture/basic/purple.png", 30,30 },
	{ "Data/Texture/basic/black.png", 30,30 },
	{ "Data/Texture/basic/gray.png", 30,30 },
	{ "Data/Texture/basic/white.png", 30,30 },
	{ "Data/Texture/basic/red.png", 30,30 },
	{ "Data/Texture/basic/question.png", 30,30 },
	{ "Data/Texture/basic/field00.png", 30,30 },

	{ "Data/Texture/UI/number.png", 400,50 },
	{ "Data/Texture/UI/moji.png",900,900 },
	{ "Data/Texture/UI/controller.png",900,600 },
	{ "Data/Texture/UI/ui.png",300,400 },
	{ "Data/Texture/UI/hpbar.png",100,100 },
	{ "Data/Texture/UI/title.png",400,148 },
	{ "Data/Texture/UI/townbg.png",800,600 },
	{ "Data/Texture/UI/selectblock.png",300,300 },
	{ "Data/Texture/UI/selectblock_b.png",300,300 },
	{ "Data/Texture/UI/ball.png",300,300 },
	{ "Data/Texture/UI/ball_w.png",300,300 },
	{ "Data/Texture/UI/menu.png",400,200 },
	{ "Data/Texture/UI/endmenu.png",600,600 },
};

enum TEXTURE_ENUM
{
	TEX_OJI,
	TEX_FIELD,
	TEX_FIELD2,
	TEX_BULLET,
	TEX_SHADOW,
	TEX_EXPLOSION,
	TEX_KUMO,
	TEX_MO,

	C_PINK,
	C_GREEN,
	C_BLUE,
	C_PURPLE,
	C_BLACK,
	C_GRAY,
	C_WHITE,
	C_RED,
	C_QUES,
	C_PLUS,

	TEX_NUMBER,
	TEX_MOJI,
	TEX_CTRL,
	TEX_UI,
	TEX_HP,
	TEX_TITLE,
	TEX_TOWNBG,
	TEX_SELECT,
	TEX_SELECTB,
	TEX_BALL,
	TEX_BALLW,
	TEX_MENU,
	TEX_ENDMENU,

	TEX_MAX,
};


class Texture
{
public:
	bool Init();
	void Uninit();
	void Set(TEXTURE_ENUM TexNum);

	LPDIRECT3DTEXTURE9 GetTex(int i) { return g_pTexture[i]; }

	static Texture* Inst() {
		if (instance == nullptr) { instance = new Texture; }
		return instance;
	}
private:
	static Texture* instance;

	LPDIRECT3DTEXTURE9 g_pTexture[TEX_MAX];
	TEXTURE_ENUM g_TextureNum = TEX_MAX;
};

#endif