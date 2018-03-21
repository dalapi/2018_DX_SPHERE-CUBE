//=============================================================================
//
// ���͏��� [input.cpp]
//
//=============================================================================
#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>	// ���߂Ďg����I
#include "Controller.h"

Controller* Controller::instance = nullptr;


BOOL CALLBACK Controller::ErumJoystickCallback(const DIDEVICEINSTANCE *pdidInstrance, VOID* pContext)
{
	HRESULT hr = instance->g_pInput->CreateDevice(pdidInstrance->guidInstance, &instance->g_pDevController, NULL);

	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

BOOL CALLBACK Controller::ErumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - CTRL_AXIS_RANGE;
	diprg.lMax = 0 + CTRL_AXIS_RANGE;
	hr = instance->g_pDevController->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) { return DIENUM_STOP; }
	return DIENUM_CONTINUE;
}
//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT Controller::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	hWnd = hWnd;
	// UNREFERENCED_PARAMETER(hWnd);	// Windows��p�A������g��Ȃ��̈Ӗ�����

	if(g_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void Controller::UninitInput(void)
{
	if(g_pInput != NULL)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT Controller::Init(HINSTANCE hInstance, HWND hWnd)
{
	use = false;
	// ���͏����̏�����
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, Controller::ErumJoystickCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		MessageBox(hWnd, "�R���g���[���[���˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	if (g_pDevController == nullptr) { return E_FAIL; }

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevController->SetDataFormat(&c_dfDIJoystick)))
	{
		MessageBox(hWnd, "�R���g���[���[�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if(FAILED(g_pDevController->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
													  // DISCL_BACKGROUND�@����g���ƁA�ʃE�B���h�E�ł̓����������ɉe������
	{
		MessageBox(hWnd, "�R���g���[���[�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �\���L�[�͈͎̔w��
	if (FAILED(g_pDevController->EnumObjects(ErumAxesCallback, hWnd, DIDFT_AXIS)))
	{
		MessageBox(hWnd, "�R���g���[���[�͈̔͂��w��ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}



	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	// �d�v�I�I�I�e�L�X�g������
	// �n�[�h�E�F�A�𓾂�
	if (FAILED(g_pDevController->Poll()))
	{
		HRESULT hr;
		hr = g_pDevController->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = g_pDevController->Acquire();
		}
	}

	use = true;
	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void Controller::Uninit(void)
{
	if(g_pDevController != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevController->Unacquire();
			
		g_pDevController->Release();
		g_pDevController = NULL;
	}

	// ���͏����̏I������
	UninitInput();

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����newKey
//=============================================================================
void Controller::Update(void)
{
	if (this->g_pDevController == nullptr) { return; }

	DIJOYSTATE newState;
	
	if (SUCCEEDED(g_pDevController->Poll()))
	{
		if (SUCCEEDED(g_pDevController->GetDeviceState(sizeof(DIJOYSTATE), &newState)))
		{
			state.lRx = newState.lRx;
			state.lRy = newState.lRy;
			state.lRz = newState.lRz;

			state.lX = newState.lX;
			state.lY = newState.lY;
			state.lZ = newState.lZ;

			for (int i = 0; i < 14; i++)
			{
				button[i].trigger = (state.rgbButtons[i] ^ newState.rgbButtons[i]) & newState.rgbButtons[i];
				button[i].release = (state.rgbButtons[i] ^ newState.rgbButtons[i]) & ~newState.rgbButtons[i];

				if (newState.rgbButtons[i])
				{
					if (button[i].repeatCnt <= LIMIT_COUNT_REPEAT)
					{
						button[i].repeatCnt++;

						if (button[i].repeatCnt == 1)
						{
							button[i].repeat = newState.rgbButtons[i];
						}
						else { button[i].repeat = 0; }
					}
					else{ button[i].repeatCnt = 0;}
				}
				else
				{
					button[i].repeatCnt = 0;
					button[i].repeat = 0;
				}

				state.rgbButtons[i] = newState.rgbButtons[i];
			}
			state.rgdwPOV[0] = newState.rgdwPOV[0];
		}
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool Controller::Press(int nKey)
{
	if (nKey <= CTRL_PAD)
	{
		return (state.rgbButtons[nKey] & 0x80) ? true : false;
	}
	else if (nKey <= CTRL_LEFT)
	{
		return (state.rgdwPOV[0] == 9000 * (nKey - CTRL_UP)) ? true : false;
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool Controller::Trigger(int nKey)
{
	if (nKey <= CTRL_PAD)
	{
		return (button[nKey].trigger & 0x80) ? true : false;
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool Controller::Repeat(int nKey)
{
	if (nKey <= CTRL_PAD)
	{
		return (button[nKey].repeat & 0x80) ? true : false;
	}
	return false;
	//	return (state[nKey].repeat & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool Controller::Release(int nKey)
{
	if (nKey <= CTRL_PAD)
	{
		return (button[nKey].release & 0x80) ? true : false;
	}
	return false;
}

void Controller::GUIUpdate(void)
{
	ImGui::Text("lRx %4d, lRy %4d, lRz %4d", state.lRx, state.lRy, state.lRz);
	ImGui::Text("lX  %4d, lY  %4d, lZ  %4d", state.lX, state.lY, state.lZ);

	ImGui::Text("button");
	for (int i = 0; i < 32; i++)
	{
		if (state.rgbButtons[i] & 0x80)
		{
			ImGui::SameLine();
			ImGui::Text("%d", i);
		}
	}
	ImGui::Text("POV");
	for (int i = 0; i < 4; i++)
	{
		if (state.rgdwPOV[0] == 9000 * i)
		{
			ImGui::SameLine();
			ImGui::Text("%d", i);
		}
	}
}
