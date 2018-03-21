#include "UI.h"
#include "Timer.h"
#include "Score.h"
#include "polygon.h"
#include "Sprite.h"
#include "Player.h"
#include "Number.h"

// 位置調整用
#define UNDER (500)
#define RIGHT (650)

UI* UI::instance = nullptr;

UI::UI()
{
	bStart = false;

	D3DXMatrixIdentity(&mStartLabel);
	D3DXMATRIX scale, trans;

	// スタートラベル位置設定する
	D3DXMatrixScaling(&scale, 20.0f, 4.0f, 4.0f);
	D3DXMatrixTranslation(&trans, 3.0f, 0.0f, -10.0f);
	mStartLabel *= scale*trans;
}


UI::~UI()
{
}

void UI::Init()
{
	Timer::Inst()->Init();
	Score::Inst()->Init();
}

void UI::Uninit()
{
	Timer::Inst()->Uninit();

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void UI::Update()
{
	// 開始フラグ立てから更新
	if (bStart) { Timer::Inst()->Update(); }
	bStart = Player::Inst()->GetStart();
}

void UI::Draw()
{
	if (bStart)
	{
		Timer::Inst()->Draw(3, RIGHT + 0, UNDER + 0);
		Score::Inst()->Draw(3, RIGHT + 0, UNDER + 40);

		// タイマー
		SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 220));
		SetPolygonScale(0.7, 0.7);
		Set2DTexture(TEX_UI);
		DrawPolygon(RIGHT - 150, UNDER + 10, 0, 350, 300, 50, false);

		// スコア
		SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 220));
		SetPolygonScale(0.7, 0.7);
		Set2DTexture(TEX_UI);
		DrawPolygon(RIGHT - 150, UNDER + 50, 0, 200, 300, 50, false);

		// プレイヤー状態
		SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 220));
		SetPolygonScale(0.5, 0.5);
		Set2DTexture(TEX_HP);
		DrawPolygon(10, UNDER + 40, 0, 0, 100, 100, false);

		// 大きさを戻る
		SetPolygonScale(0.9, 0.9);
		Number::DrawNumber(Player::Inst()->GetLife(), 60, UNDER + 40);

		// 大きさを戻る
		SetPolygonScale(1.0, 1.0);
		
	}
	else
	{
		Sprite::Inst()->Draw(TEX_UI, mStartLabel, 0, 100, 600, 100, 600, 800);
	}
}