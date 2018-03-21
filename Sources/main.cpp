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
// ���������[�N�`�F�b�N
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// �}�N����`
#define CLASS_NAME		"SAMPLE"
#define WINDOW_NAME		"-SPHERE CUBE-"
#define SCREEN_STYLE	WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX^WS_MAXIMIZEBOX

// �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool Init(HINSTANCE hInstance, HWND& hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void ImGuiUpdate(void);
void Draw(void);

// �O���o�[���ϐ�
static LPDIRECT3D9		g_pD3D = NULL;				// DIRECT3D �C���^�t�F�[�X
static LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// DIRECT3D �f�o�C�X
static bool bGui = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ���C���֐� 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	WNDCLASSEX wecx = {
		sizeof(WNDCLASSEX),			// ���̍\���́i���g�j�̃T�C�Y
		CS_VREDRAW | CS_HREDRAW,	// style
		WndProc,					// �A�h���X�̂��ƁF��Ń|�C���^�����܂�
		0,
		0,
		hInstance,					// WinMain�̃C���X�^���X�n���h��
		NULL,						// �A�C�R���n���h���i����Ȃ��j
		LoadCursor(NULL,IDC_ARROW), // �J�[�\���n���h��
		(HBRUSH)(COLOR_WINDOW+1),	// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F
		NULL,						// ���j���[
		CLASS_NAME,					// �N���X�l�[���F��ŕϐ������
		NULL,						// �~�j�A�C�R��

	};								// �\���̂̏�����

	RegisterClassEx(&wecx);			// �E�B���h�E�N���X�̓o�^�i�e�E�B���h�E�j

	UnregisterClass(_T("ImGui Example"), wecx.hInstance);
	
	RECT wr = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	AdjustWindowRect(&wr, SCREEN_STYLE, false); // ���̊֐��ŃT�C�X�̐��m���m�F

	RECT dr;
	GetWindowRect(GetDesktopWindow(),&dr); // �p�\�R���̉�ʂ̉�͓x

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

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	DWORD dwExceLastTime = 0;
	DWORD dwCurrentTime = 0;

	timeBeginPeriod(1);

	// ������
	Init(hInstance, hWnd, TRUE);

	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{// �E�B���h�E���b�Z�[�W����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	
			dwCurrentTime = timeGetTime();// �~���b�P��
			if ((dwCurrentTime - dwExceLastTime) * 60 >= 1000)

			{// �Q�[�������@�Q�[�����[�v
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
		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�̑��M�i������Ԃ���������Ǘ��҂Ƀ��b�Z�[�W�𑗂�Ȃǁj
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		//case WM_RBUTTONDOWN:	// �E�N���b�N�����
		//case VK_RETURN:		//�G���^�[�L�[�������ꂽ��
		case VK_ESCAPE:			//ESC�L�[�������ꂽ��

		nID = MessageBox(hWnd, "�A�v���P�[�V�������I�����܂����H", "SPHERE CUBE", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	// �E�B���h�E��߂�
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

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL){ return false;}
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {return false;}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp)); //�[���ŏ�����
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
	

	// �f�o�C�X�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>��
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice))) // important!!
	{
		MessageBox(hWnd, "�f�o�C�X�������s�B", "DirectX", MB_OK);
			return false;
	}

	// ���ݒ�
	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// ���u�����h���s��
	// SRC�c������`�����́B�܂�|���S���Ƀe�N�X�`����\��������
	// DESR�c���łɕ`�悳��Ă����ʂ̂��ƁB
	// SRC_RGB * SRC_�� + DEST_RGB * (1-SRC_��)
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// ��

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// UV���W
	// �e�N�X�`���A�h���X�O���Q�Ƃ������ǂ����邪�ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); // ��͓x���ς��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); // �g��k���̏ꍇ��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); // �����Y��łȂ�܂�

	// ���e�X�g
	g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 150); 
	g_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ������
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

	if (g_pD3DDevice != NULL) { // �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if (g_pD3D != NULL) { // Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

void Update(void)
{
	Input::Inst()->Update();
	Controller::Inst()->Update();
	SceneManager::Inst()->Update();

	// ImGui�\��
	if (Input::Inst()->Trigger(DIK_F1)) { bGui = !bGui; }
}

void Draw(void)
{
	g_pD3DDevice->Clear( 0, NULL,(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// ��ʂ̃N���A
		D3DCOLOR_RGBA(0, 158, 180, 155), 1.0f, 0);						// ���F�ŃN���A����

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{


		// ���V�[���̕`��
		SceneManager::Inst()->Draw();

		// �G�t�F�N�g�̕`��J�n�������s���B
		Effects::Inst()->GetRender()->BeginRendering();

		// �G�t�F�N�g�̕`����s���B
		Effects::Inst()->Draw();

		// �G�t�F�N�g�̕`��I���������s���B
		Effects::Inst()->GetRender()->EndRendering();

		// ImGui�\��
		if (bGui){ ImGui::Render(); }

		

		g_pD3DDevice->EndScene(); // Direct3D�ɂ��`��̏I��
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
