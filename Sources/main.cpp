#include "main.h"
#include "input.h"
#include "Controller.h"
#include "Texture.h"
#include "SceneManager.h"
#include "polygon.h"
#include "sound.h"
#include "xmodel.h"

#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_dx9.h"
#include <tchar.h>
#include "GUI.h"

#include "Effects.h"	// Effekseer


#if _DEBUG
// メモリリークチェック
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// マクロ定義
#define CLASS_NAME		"SAMPLE"
#define WINDOW_NAME		"-SPHERE CUBE-"
#define SCREEN_STYLE	WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX^WS_MAXIMIZEBOX

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool Init(HINSTANCE hInstance, HWND& hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void ImGuiUpdate(void);
void Draw(void);

// グロバール変数
static LPDIRECT3D9		g_pD3D = NULL;				// DIRECT3D インタフェース
static LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// DIRECT3D デバイス
static bool bGui = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// メイン関数 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	WNDCLASSEX wecx = {
		sizeof(WNDCLASSEX),			// この構造体（自身）のサイズ
		CS_VREDRAW | CS_HREDRAW,	// style
		WndProc,					// アドレスのこと：後でポインタを作ります
		0,
		0,
		hInstance,					// WinMainのインスタンスハンドル
		NULL,						// アイコンハンドル（今回なし）
		LoadCursor(NULL,IDC_ARROW), // カーソルハンドル
		(HBRUSH)(COLOR_WINDOW+1),	// ウインドウのクライアント領域の背景色
		NULL,						// メニュー
		CLASS_NAME,					// クラスネーム：後で変数を作る
		NULL,						// ミニアイコン

	};								// 構造体の初期化

	RegisterClassEx(&wecx);			// ウィンドウクラスの登録（親ウィンドウ）

	UnregisterClass(_T("ImGui Example"), wecx.hInstance);
	
	RECT wr = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	AdjustWindowRect(&wr, SCREEN_STYLE, false); // この関数でサイスの正確を確認

	RECT dr;
	GetWindowRect(GetDesktopWindow(),&dr); // パソコンの画面の解析度

	HWND hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		SCREEN_STYLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	// ウィンドウの表示
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	DWORD dwExceLastTime = 0;
	DWORD dwCurrentTime = 0;

	timeBeginPeriod(1);

	// 初期化
	Init(hInstance, hWnd, TRUE);

	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{// ウィンドウメッセージ処理
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	
			dwCurrentTime = timeGetTime();// ミリ秒単位
			if ((dwCurrentTime - dwExceLastTime) * 60 >= 1000)

			{// ゲーム処理　ゲームループ
				Update();
				Draw();

				dwExceLastTime = dwCurrentTime;
			}
		}

		ImGuiUpdate();

	}while(msg.message != WM_QUIT);

	timeEndPeriod(1);
	Uninit();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	PAINTSTRUCT ps;
	HDC hDC;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) { return true; }

	switch (uMsg)
	{

	case WM_SIZE:
		if (g_pD3DDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			D3DPRESENT_PARAMETERS d3dpp;

			ImGui_ImplDX9_InvalidateDeviceObjects();
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = g_pD3DDevice->Reset(&d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();

		}
		return 0;

	case WM_PAINT:		
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0); // WM_QUITメッセージの送信（何か状態があったら管理者にメッセージを送るなど）
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		//case WM_RBUTTONDOWN:	// 右クリックすると
		//case VK_RETURN:		//エンターキーを押されたら
		case VK_ESCAPE:			//ESCキーを押されたら

		nID = MessageBox(hWnd, "アプリケーションを終了しますか？", "SPHERE CUBE", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	// ウィンドウを閉める
			}
			break;	
		}
		break;

		default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Init(HINSTANCE hInstance, HWND& hWnd, BOOL bWindow)
{	
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL){ return false;}
	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {return false;}

	// デバイスのプレゼンテーションパラメータの設定
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp)); //ゼロで初期化
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat			 = d3ddm.Format;
	d3dpp.BackBufferCount			 = 1;
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil	 = TRUE;
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;
	d3dpp.Windowed					 = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;
	

	// デバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>を
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice))) // important!!
	{
		MessageBox(hWnd, "デバイス生成失敗。", "DirectX", MB_OK);
			return false;
	}

	// α設定
	// レンダーステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// αブレンドを行う
	// SRC…今から描くもの。つまりポリゴンにテクスチャを貼ったもの
	// DESR…すでに描画されている画面のこと。
	// SRC_RGB * SRC_α + DEST_RGB * (1-SRC_α)
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// α
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// α

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// UV座標
	// テクスチャアドレス外を参照した時どうするが設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); // 解析度が変わる
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); // 拡大縮小の場合は
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); // 線が綺麗でなります

	// αテスト
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 150); 
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 初期化
	ImGui_ImplDX9_Init(hWnd, g_pD3DDevice);
	Sound::Inst()->Init(hWnd);
	Input::Inst()->Init(hInstance, hWnd);
	Controller::Inst()->Init(hInstance, hWnd);
	Effects::Inst()->Init();
	Texture::Inst()->Init();
	InitPolygon();
	Model::Inst()->Init();
	SceneManager::Inst()->Init();

	return S_OK;
}

void Uninit(void)
{
	UninitPolygon();
	Texture::Inst()->Uninit();
	Model::Inst()->Uninit();
	Controller::Inst()->Uninit();
	Input::Inst()->Uninit();
	Effects::Inst()->Uninit();
	Sound::Inst()->Stop();
	Sound::Inst()->Uninit();
	SceneManager::Inst()->Uninit();

	ImGui_ImplDX9_Shutdown();

	if (g_pD3DDevice != NULL) { // デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if (g_pD3D != NULL) { // Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

void Update(void)
{
	Input::Inst()->Update();
	Controller::Inst()->Update();
	SceneManager::Inst()->Update();

	// ImGui表示
	if (Input::Inst()->Trigger(DIK_F1)) { bGui = !bGui; }
}

void Draw(void)
{
	g_pD3DDevice->Clear( 0, NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// 画面のクリア
		D3DCOLOR_RGBA(0, 158, 180, 155), 1.0f, 0);						// 何色でクリアする

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{


		// 今シーンの描画
		SceneManager::Inst()->Draw();

		// エフェクトの描画開始処理を行う。
		Effects::Inst()->GetRender()->BeginRendering();

		// エフェクトの描画を行う。
		Effects::Inst()->Draw();

		// エフェクトの描画終了処理を行う。
		Effects::Inst()->GetRender()->EndRendering();

		// ImGui表示
		if (bGui){ ImGui::Render(); }

		

		g_pD3DDevice->EndScene(); // Direct3Dによる描画の終了
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DDEVICE9 GetD3DDevice(void)
{
	return g_pD3DDevice;
}

void ImGuiUpdate(void)
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec2 pos = { 30, 30 };
	ImGui::SetNextWindowPos(pos);
	ImGui_ImplDX9_NewFrame();

	for (int i = 0; i < MAX_GUI; i++)
	{
		if (GUI::Get(i) != nullptr)
		{
			GUI::Get(i)->GUIUpdate();
		}
	}

	//	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	//ImGui::Text("Hello, World!");
}
