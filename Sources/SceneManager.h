#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "BaseScene.h"
#include "GUI.h"

enum SCENE {
	TITLE,
	TUTORIAL,
	GAME,
	RESULT
};

class SceneManager :public GUI
{
public:

	~SceneManager();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Fade() { bFade = 1; } // ���Ԓʂ�
	void Fade(SCENE scene) { bFade = 1; Scene = scene; } // ��O�̃V�[���w�肷��
	void ChangeScene(SCENE changer);
	SCENE GetScene() { return Scene; }

	void GUIUpdate();
	
	SCENE Scene;

	static SceneManager* Inst()
	{
		if (instance == nullptr) { instance = new SceneManager; }
		return instance;
	}

private:
	static BaseScene* m_pScene; // ���̃V�[��
	static SceneManager* instance;

	bool bChange, bFade;
	int nAlpha;
};

#endif