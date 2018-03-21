#include "SceneManager.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "input.h"
#include "polygon.h"

// シーン切り替え用
BaseScene *SceneManager::m_pScene = nullptr;
SceneManager* SceneManager::instance = nullptr;


void SceneManager::ChangeScene(SCENE Sc)
{
	if (m_pScene != nullptr) 
	{
		delete m_pScene;

		switch (Sc)
		{
		case TITLE:
			Scene = TITLE;
			m_pScene = new TitleScene();
			break;
		case TUTORIAL:
			Scene = TUTORIAL;
			m_pScene = new TutorialScene();
			break;
		case GAME:
			Scene = GAME;
			m_pScene = new GameScene();
			break;
		case RESULT:
			Scene = RESULT;
			m_pScene = new ResultScene();
			break;
		}

		bChange = false;
	}
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	// test
	Scene = TITLE;
	m_pScene = new TitleScene();

	// フェイドアウト用（フェイドインまだ）
	bFade = bChange = false;
	nAlpha = 0;
}

void SceneManager::Uninit()
{
	if (m_pScene != nullptr)
	{
		delete m_pScene;
		m_pScene = nullptr;
	}
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void SceneManager::Update()
{
	// test

	if (bFade)
	{
		if (nAlpha < 255) { nAlpha += 3;; }
		else { 
			bFade = false;
			bChange = true;
			nAlpha = 0;
		}
	}
	if(bChange)
	{
		if (Scene == TITLE) { ChangeScene(TUTORIAL); }
		else if (Scene == TUTORIAL) { ChangeScene(GAME); }
		else if (Scene == GAME) { ChangeScene(RESULT); }
		else if (Scene == RESULT) { ChangeScene(TITLE); }
	}

	m_pScene->Update();
}

void SceneManager::Draw()
{	
	m_pScene->Draw();

	if (bFade){ 
		SetPolygonColor(D3DCOLOR_RGBA(0, 0, 0, nAlpha));
		Set2DTexture(TEX_OJI);
		DrawPolygon(0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false);
	}
}

void SceneManager::GUIUpdate()
{
	ImGui::Text("SceneManager");
	ImGui::Text("Fade Alpha %3d", nAlpha);
	ImGui::Text("Now Scene  %2d", Scene);
}