#include "SceneManager.h"
#include "GameScene.h"
#include "FieldController.h"
#include "xmodel.h"
#include "Camera.h"
#include "light.h"
#include "Player.h"
#include "Object.h"
#include "Enemy.h"
#include "polygon.h"
#include "BackGround.h"
#include "UI.h"
#include "Timer.h"
#include "Enemy.h"
#include "Cube.h"
#include "Sprite.h"
#include "Map.h"
#include "Effects.h"

#include "sound.h"
#include "input.h"
#include "Controller.h"

GameScene::GameScene() 
{
	Init();
}
GameScene::~GameScene()
{
	Uninit();
}

void GameScene::Init()
{
	Sound::Inst()->Play(BGM_GAME);
	Player::Inst()->Init();
	FieldController::Inst()->Init();
	Camera::Inst()->Init(GAME);
	UI::Inst()->Init();
	Map::Inst()->Init();

	bPause = bStart = bEnd = bGoal = 0;
	nAnimCount, nPattern = 0;
}

void GameScene::Uninit()
{
	//Effects::Inst()->Stop();
	BackGround::Inst()->Uninit();
	Object::UninitObject();
	Camera::Inst()->Uninit();
	Player::Inst()->Uninit();
	CreateEnemy::Inst()->Uninit();
	FieldController::Inst()->Uninit();
	Cube::Inst()->Uninit();
	Sprite::Inst()->Uninit();
	UI::Inst()->Uninit();
	Map::Inst()->Uninit();
}

void GameScene::Update()
{
	if(!bPause && !bEnd && !bGoal)
	{
		UI::Inst()->Update();
		Player::Inst()->Update();
		FieldController::Inst()->Update();
		Camera::Inst()->Update();
		BackGround::Inst()->Update();
		CreateEnemy::Inst()->Update();
		UpdateLight();
		Object::UpdateObject();
		bEnd = Player::Inst()->GetEnd();
		bGoal = Player::Inst()->GetGoal();
	}
	else if(bPause && !bEnd && !bGoal)
	{
		PauseUpdate();
	}
	else if (bEnd || bGoal) { EndUpdate(); }

	bEnd = Timer::Inst()->EndFlag();


	//ポース
	if (Input::Inst()->Trigger(DIK_P) || Controller::Inst()->Trigger(CTRL_OPTION)) { bPause = !bPause; }

	nAnimCount++;
}

void GameScene::Draw() 
{
	BackGround::Inst()->Draw();
	FieldController::Inst()->Draw();
	Player::Inst()->Draw();
	Object::DrawObject();
	if (bEnd || bGoal) { EndDraw(); }else{ UI::Inst()->Draw(); }

	//ポース画面
	if (bPause && !bEnd) { PauseDraw(); }
}

void GameScene::PauseUpdate()
{

}
void GameScene::PauseDraw()
{
	// 背景
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 155));
	Set2DTexture(C_BLACK);
	DrawPolygon(0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	// マップ
	//Map::Inst()->Draw();

	// 
	//SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	//Set2DTexture(TEX_CTRL);
	//DrawPolygon((SCREEN_WIDTH-900)/2, (SCREEN_HEIGHT-600)/2, 0, 0, 900, 600, false);

	// 選択用ブロック
	nPattern = nAnimCount / 4; // SPEED
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_SELECT);
	DrawPolygon(SCREEN_WIDTH / 2.0f-150.0f, (SCREEN_HEIGHT / 2.0f)-30.0f, 0, 100 * (nPattern % 3), 300, 100, false);

	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_UI);
	DrawPolygon(SCREEN_WIDTH / 2.0f-100.0f, (SCREEN_HEIGHT / 2.0f)-25.0f, 0, 300, 300, 50, false);

}

void GameScene::EndUpdate()
{
	Camera::Inst()->Update();

	if (Input::Inst()->Trigger(DIK_RETURN) || Controller::Inst()->Trigger(CTRL_MARU))
	{
		Effects::Inst()->Stop();
		Sound::Inst()->Play(SE_DD);
		SceneManager::Inst()->Fade();
		Sound::Inst()->Stop(BGM_GAME);
	}

	Effects::Inst()->Update();
}

void GameScene::EndDraw()
{
	// bg
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 155));
	Set2DTexture(C_BLACK);
	DrawPolygon(0, 100, 0, 0, SCREEN_WIDTH, 400, false);


	if (Player::Inst()->GetLife() <= 0)
	{
		// Life Over
		SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		Set2DTexture(TEX_ENDMENU);
		DrawPolygon((SCREEN_WIDTH / 2.0f) - 300, (SCREEN_HEIGHT / 2.0f) - 50, 0, 0, 600, 100, false);
	}
	else if (Timer::Inst()->EndFlag())
	{
		// Time Up
		SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		Set2DTexture(TEX_ENDMENU);
		DrawPolygon((SCREEN_WIDTH / 2.0f) - 300, (SCREEN_HEIGHT / 2.0f) - 50, 0, 100, 600, 100, false);
	}
	else if (bGoal)
	{
		// Goal
		SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		Set2DTexture(TEX_ENDMENU);
		DrawPolygon((SCREEN_WIDTH / 2.0f) - 300, (SCREEN_HEIGHT / 2.0f) - 50, 0, 200, 600, 100, false);
	}
}