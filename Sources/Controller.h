//=============================================================================
//
// 入力処理 [input.h]
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#define CTRL_AXIS_RANGE (100)
#define LIMIT_COUNT_REPEAT (20)

#define DIRECTINPUT_VERSION (0x0800)
#define CTRL_SHIKAKU	(0)
#define CTRL_BATSU		(1)
#define CTRL_MARU		(2)
#define CTRL_SANKAKU	(3)
#define CTRL_L1			(4)
#define CTRL_R1			(5)
#define CTRL_L2			(6)
#define CTRL_R2			(7)
#define CTRL_SHARE		(8)
#define CTRL_OPTION		(9)
#define CTRL_L3			(10)
#define CTRL_R3			(11)
#define CTRL_PS			(12)
#define CTRL_PAD		(13)

#define CTRL_UP			(14)
#define CTRL_RIGHT		(15)
#define CTRL_DOWN		(16)
#define CTRL_LEFT		(17)

#include "main.h"
#include <dinput.h>

#include "GUI.h"

class Controller : public GUI
{
	typedef struct
	{
		BYTE  trigger;
		BYTE  release;
		BYTE  repeat;
		SHORT repeatCnt;
	}CTRL_BUTTON;
public:
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool Press(int nKey);
	bool Trigger(int nKey);
	bool Repeat(int nKey);
	bool Release(int nKey);

	D3DXVECTOR2 LeftStick(void)
	{
		return D3DXVECTOR2((float)state.lX, (float)state.lY);
	}

	D3DXVECTOR2 RightStick(void)
	{
		return D3DXVECTOR2((float)state.lZ, (float)state.lRz);
	}

	void GUIUpdate(void);
	bool GetUse(void) { return use; }

	// シングルトン
	static Controller* Inst() 
	{
		if(instance == nullptr)
		{
			instance = new Controller;
		}
		return instance;
	}

private:
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	void UninitInput(void);

	LPDIRECTINPUT8			g_pInput = NULL;					// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8	g_pDevController = NULL;				// 入力デバイス(キーボード)へのポインタ

	DIJOYSTATE state;
	DWORD   rgdwPOV[4];

	CTRL_BUTTON button[14];
	bool use;

	static Controller* instance;
	static BOOL CALLBACK ErumJoystickCallback(const DIDEVICEINSTANCE *pdidInstrance, VOID* pContext);
	static BOOL CALLBACK ErumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidol, VOID *pContext);
};

#endif