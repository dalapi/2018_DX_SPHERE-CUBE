//=============================================================================
//
// 入力処理 [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

#include "GUI.h"
//
#define	NUM_KEY_MAX			(256)	// キーの最大数
#define	LIMIT_REPEAT	(30)	// リピートカウントリミッター

class Input : public GUI
{
	typedef struct
	{
		BYTE press;			// キーボードの入力情報ワーク
		BYTE trigger;	// キーボードのトリガー情報ワーク
		BYTE release;	// キーボードのリリース情報ワーク
		BYTE repeat;		// キーボードのリピート情報ワーク
		int  repeatCnt;	// キーボードのリピートカウンタ
	}KEY_STATE;
public:
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void GUIUpdate(void);

	bool Press(int nKey);
	bool Trigger(int nKey);
	bool Repeat(int nKey);
	bool Release(int nKey);

	// シングルトン
	static Input* Inst() 
	{
		if (instance == nullptr) { instance = new Input; }
		return instance;
	}

private:
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	void UninitInput(void);

	LPDIRECTINPUT8			g_pInput = NULL;					// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// 入力デバイス(キーボード)へのポインタ

	KEY_STATE state[NUM_KEY_MAX];

	static Input* instance;
};

#endif