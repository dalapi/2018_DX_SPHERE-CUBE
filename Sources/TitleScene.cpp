#include "SceneManager.h"
#include "TitleScene.h"
#include "polygon.h"
#include "Cube.h"
#include "FieldController.h"
#include "Camera.h"
#include "light.h"
#include "Object.h"

#include "Controller.h"
#include "input.h"
#include "sound.h"


#define POS_RIGHT (310)
#define POS_UNDER (450)

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
	Uninit();
}

void TitleScene::Init()
{
	Sound::Inst()->Play(BGM_TITLE);
	Camera::Inst()->Init(TITLE);
	FieldController::Inst()->Init();

	// メニュー選択用
	nChoose = nAnimCount = nPattern = 0;
	fBlockPos = 0.0f;
}

void TitleScene::Uninit()
{
	FieldController::Inst()->Uninit();
	Camera::Inst()->Uninit();
	Cube::Inst()->Uninit();
	Object::UninitObject();
}

void TitleScene::Update()
{
	// メニュー選択
	if (Input::Inst()->Trigger(DIK_DOWN) || Controller::Inst()->Trigger(CTRL_DOWN))
	{
		Camera::Inst()->SetRepeat();
		Sound::Inst()->Play(SE_CLICK);
		if (nChoose < 2) { nChoose++; } else { nChoose = 0; }
	}

	if (Input::Inst()->Trigger(DIK_RETURN) || Controller::Inst()->Trigger(CTRL_MARU))
	{ 
		Sound::Inst()->Play(SE_CHECK);
		SceneManager::Inst()->Fade(); 
		Sound::Inst()->Stop(BGM_TITLE);
	}


	// メニュー選択ブロック位置
	switch (nChoose)
	{
	case 0:
		fBlockPos = 0.0f;
		//Camera::Inst()->Update();
		Camera::Inst()->UpdateAt(10, UP);
		Camera::Inst()->UpdatePos(30, RIGHT);
		break;
	case 1:
		fBlockPos = 50.0f;
		Camera::Inst()->UpdateAt(30, UP);
		Camera::Inst()->UpdatePos(20, RIGHT);
		break;
	case 2:
		fBlockPos = 100.0f;
		Camera::Inst()->UpdateAt(40, DOWN);
		Camera::Inst()->UpdatePos(50, LEFT);
		break;
	}
	

	FieldController::Inst()->Update();
	UpdateLight();

	if (nAnimCount < 1600) { nAnimCount++; }
	else { nAnimCount = 0; }
}

void TitleScene::Draw()
{
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_MO);
	DrawPolygon(0, 0, nAnimCount, 0, SCREEN_WIDTH*2.0f, (float)SCREEN_HEIGHT, false);

	FieldController::Inst()->Draw();

	// 選択用ブロック
	nPattern = nAnimCount / 5; // SPEED
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_SELECT);
	DrawPolygon(POS_RIGHT - 50, POS_UNDER-30 /*+ fBlockPos*/, 0, 100*(nPattern % 3), 300, 100, false);

	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_BALL);
	SetPolygonRotation(150.0f, 150.0f, nAnimCount);
	DrawPolygon((SCREEN_WIDTH / 2.0f) - 150, -30, 0, 0, 300, 300, 1);

	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_TITLE);
	DrawPolygon((SCREEN_WIDTH/2.0f)-200, 50, 0, 0, 400, 148, false);

	// start/option/exit
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_UI);
	DrawPolygon(POS_RIGHT, POS_UNDER + 0, 0, 50 /*+ fBlockPos*/, 300, 50, false);

}