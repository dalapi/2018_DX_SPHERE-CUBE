#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
#include "GUI.h"
#include "SceneManager.h"

enum POS
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

class Camera : public GUI
{
public:
	void Init(SCENE sc);
	void Uninit();
	void Update();
	void UpdateAt(int count,POS pos);
	void UpdatePos(int count, POS pos);
	void GUIUpdate();
	void SetRepeat() { bRepeatAt = true; bRepeatPos = true; }

	D3DXVECTOR3 GetAt() { return g_at; }
	D3DXVECTOR3 GetEye() { return g_eye; }

	static Camera* Inst() {
		if (instance == nullptr) { instance = new Camera; }
		return instance;
	}

private:
	static Camera* instance;

	// グロバール変数
	D3DXVECTOR3 g_eye, g_at, g_up;
	D3DXVECTOR3 g_front, g_right; //必ず単位ベクトル（正規化必要）

	// ビュー行列とプロジェクション行列
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxProjection;

	// 回転行列
	D3DXMATRIX mtxRot;

	int repeatAt,repeatPos;
	bool bRepeatAt, bRepeatPos;

	SCENE sc;
};

#endif#pragma once
