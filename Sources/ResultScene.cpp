#include "SceneManager.h"
#include "ResultScene.h"
#include "polygon.h"
//#include "BackGround.h"
#include "Score.h"

#include "sound.h"
#include "input.h"
#include "Controller.h"

ResultScene::ResultScene()
{
	Init();
}

ResultScene::~ResultScene()
{
	Uninit();
}

void ResultScene::Init()
{
	Sound::Inst()->Play(BGM_OTHER);
	nAnimCount = nPattern = 0;
}


void ResultScene::Uninit()
{
	//BackGround::Inst()->Uninit();
	Score::Inst()->Uninit();
}

void ResultScene::Update()
{
	//BackGround::Inst()->Update();
	if (Input::Inst()->Trigger(DIK_RETURN) || Controller::Inst()->Trigger(CTRL_MARU))
	{
		Sound::Inst()->Play(SE_CHECK);
		SceneManager::Inst()->Fade();
		Sound::Inst()->Stop(BGM_OTHER);
	}
	
	nAnimCount++;
}

void ResultScene::Draw()
{
	//BackGround::Inst()->Draw();
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_MO);
	DrawPolygon(0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	// bg
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(C_BLACK);
	DrawPolygon(0, 100, 0, 0, SCREEN_WIDTH, 400, false);

	// ball
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_BALLW);
	SetPolygonRotation(150.0f, 150.0f, nAnimCount);
	DrawPolygon((SCREEN_WIDTH / 2.0f) - 150, (SCREEN_HEIGHT/2.0f) - 150, 0, 0, 300, 300, 1);

	// ƒXƒRƒA
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 220));
	Set2DTexture(TEX_UI);
	DrawPolygon((SCREEN_WIDTH / 2.0f) - 100, (SCREEN_HEIGHT / 2.0f) - 45, 0, 200, 300, 50, false);
	Score::Inst()->Draw(5, (SCREEN_WIDTH / 2.0f) - 100, (SCREEN_HEIGHT / 2.0f));

	// title(kari)
	/*
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_UI);
	DrawPolygon((SCREEN_WIDTH / 2)-300, (SCREEN_HEIGHT / 2), 0, 250, 300, 50, false);
	*/
}