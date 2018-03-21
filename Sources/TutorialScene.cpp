#include "SceneManager.h"
#include "TutorialScene.h"
#include "polygon.h"
#include "xmodel.h"
#include "Effects.h"
#include "Camera.h"

#include "Controller.h"
#include "input.h"
#include "sound.h"

TutorialScene::TutorialScene()
{
	Init();
}


TutorialScene::~TutorialScene()
{
	Uninit();
}

void TutorialScene::Init()
{
	Camera::Inst()->Init(TUTORIAL);
	Sound::Inst()->Play(BGM_OTHER);

	rot = { 0,0,0 };

	D3DXMatrixIdentity(&mtx);

	D3DXMATRIX m_trans, m_scale;
	D3DXMatrixTranslation(&m_trans, -0.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&m_scale, 10.0f, 10.0f, 10.0f);

	mtx *= m_scale *m_trans;

	// メニュー選択用
	nChoose = nAnimCount = nPattern = 0;
	fBlockPos = 0.0f;
}

void TutorialScene::Uninit()
{
	Camera::Inst()->Uninit();
}

void TutorialScene::Update()
{
	// メニュー選択
	if (Input::Inst()->Trigger(DIK_DOWN) || Controller::Inst()->Trigger(CTRL_DOWN))
	{
		Sound::Inst()->Play(SE_CLICK);
		if (nChoose < 2) { nChoose++; } else { nChoose = 0; }
	}
	if (Input::Inst()->Press(DIK_UP) || Controller::Inst()->Trigger(CTRL_UP))
	{
		Sound::Inst()->Play(SE_CLICK);
		if (nChoose > 0) { nChoose--; } else { nChoose = 2; }
	}

	if (Input::Inst()->Trigger(DIK_RETURN) || Controller::Inst()->Trigger(CTRL_MARU))
	{
		Sound::Inst()->Play(SE_CHECK);
		SceneManager::Inst()->Fade();
		Sound::Inst()->Stop(BGM_OTHER);
	}

	// メニュー選択ブロック位置
	switch (nChoose)
	{
	case 0:
		fBlockPos = -10.0f;
		break;
	case 1:
		fBlockPos = 40.0f;
		break;
	case 2:
		fBlockPos = 100.0f;
		break;
	}

	if (Input::Inst()->Trigger(DIK_N)) { Effects::Inst()->Play(PART,D3DXVECTOR3(3,3,3)); }
	if (Input::Inst()->Trigger(DIK_M)) { Effects::Inst()->Play(SQUE,D3DXVECTOR3(5,3,3)); }

	if (rot.y < 0.01) { rot.y += 0.01; }
	else { rot.y = 0; }
	if (rot.x > -0.01) { rot.x -= 0.01; }
	else { rot.x = 0; }
	
	D3DXMATRIX m_rorate;
	D3DXMatrixRotationYawPitchRoll(&m_rorate, rot.y, rot.x, rot.z);
	mtx *= m_rorate;
	
	Camera::Inst()->Update();
	Effects::Inst()->Update();

	nAnimCount++;
}

void TutorialScene::Draw()
{
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(C_GRAY);
	DrawPolygon(0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	Model::Inst()->Draw((Model::ENUM)0, mtx, 0);


	// 選択用ブロック
	nPattern = nAnimCount / 4; // SPEED
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_SELECT);
	DrawPolygon(SCREEN_WIDTH / 2.0f+ 40.0f, (SCREEN_HEIGHT/2.0f)-90.0f + fBlockPos, 0, 100 * (nPattern % 3), 300, 100, false);


	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_UI);
	DrawPolygon(50, 50, 0, 100, 300, 50, false);

	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Set2DTexture(TEX_MENU);
	DrawPolygon(SCREEN_WIDTH/2.0f, (SCREEN_HEIGHT / 2.0f)-100.0f, 0, 0, 400, 200, false);

}