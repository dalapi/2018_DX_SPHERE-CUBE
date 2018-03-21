//=============================================================================
//
// 入力処理 [input.cpp]
//
//=============================================================================
#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>	// 初めて使うやつ！
#include "input.h"

Input* Input::instance = nullptr;

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT Input::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	hWnd = hWnd;
	// UNREFERENCED_PARAMETER(hWnd);	// Windows専用、今これ使わないの意味かな

	if(g_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void Input::UninitInput(void)
{
	if(g_pInput != NULL)
	{// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT Input::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
													  // DISCL_BACKGROUND　これ使うと、別ウィンドウでの動くもこちに影響する
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	// 重要！！！テキストを見て
	// ハードウェアを得る
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void Input::Uninit(void)
{
	if(g_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	UninitInput();

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

//=============================================================================
// キーボードの更新処理newKey
//=============================================================================
void Input::Update(void)
{
	BYTE newKey[NUM_KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(newKey), newKey)))
	{
		// newKey[DIK_SPACE] & 0x80;

		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キートリガー・リリース情報を生成
			state[nCnKey].trigger = (state[nCnKey].press ^ newKey[nCnKey]) & newKey[nCnKey];
			state[nCnKey].release = (state[nCnKey].press ^ newKey[nCnKey]) & ~newKey[nCnKey];

			// キーリピート情報を生成
			if(newKey[nCnKey])
			{
				if(state[nCnKey].repeatCnt < LIMIT_REPEAT)
				{
					state[nCnKey].repeatCnt++;
					if(state[nCnKey].repeatCnt == 1)
					{// キーを押し始めた最初のフレーム、または一定時間経過したらキーリピート情報ON
						state[nCnKey].repeat = newKey[nCnKey];
					}
					else
					{
						state[nCnKey].repeat = 0;
					}
				}
				else
				{
					state[nCnKey].repeatCnt = 0;
				}
			}
			else
			{
				state[nCnKey].repeatCnt = 0;
				state[nCnKey].repeat = 0;
			}

			// キープレス情報を保存
			state[nCnKey].press = newKey[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool Input::Press(int nKey)
{
	return (state[nKey].press & 0x80) ? true: false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool Input::Trigger(int nKey)
{
	return (state[nKey].trigger & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool Input::Repeat(int nKey)
{
	return (state[nKey].repeat & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool Input::Release(int nKey)
{
	return (state[nKey].release & 0x80) ? true: false;
}

void Input::GUIUpdate(void)
{
	ImGui::Text("RepeatCnt %d", this->state[DIK_T].repeatCnt);
	if (this->Repeat(DIK_T))
	{
		ImGui::SameLine();
		ImGui::Text("push");
	}
}
