//=============================================================================
//
// ���͏��� [input.cpp]
//
//=============================================================================
#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>	// ���߂Ďg����I
#include "input.h"

Input* Input::instance = nullptr;

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT Input::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	hWnd = hWnd;
	// UNREFERENCED_PARAMETER(hWnd);	// Windows��p�A������g��Ȃ��̈Ӗ�����

	if(g_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void Input::UninitInput(void)
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
HRESULT Input::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
													  // DISCL_BACKGROUND�@����g���ƁA�ʃE�B���h�E�ł̓����������ɉe������
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	// �d�v�I�I�I�e�L�X�g������
	// �n�[�h�E�F�A�𓾂�
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void Input::Uninit(void)
{
	if(g_pDevKeyboard != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
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
void Input::Update(void)
{
	BYTE newKey[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(newKey), newKey)))
	{
		// newKey[DIK_SPACE] & 0x80;

		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// �L�[�g���K�[�E�����[�X���𐶐�
			state[nCnKey].trigger = (state[nCnKey].press ^ newKey[nCnKey]) & newKey[nCnKey];
			state[nCnKey].release = (state[nCnKey].press ^ newKey[nCnKey]) & ~newKey[nCnKey];

			// �L�[���s�[�g���𐶐�
			if(newKey[nCnKey])
			{
				if(state[nCnKey].repeatCnt < LIMIT_REPEAT)
				{
					state[nCnKey].repeatCnt++;
					if(state[nCnKey].repeatCnt == 1)
					{// �L�[�������n�߂��ŏ��̃t���[���A�܂��͈�莞�Ԍo�߂�����L�[���s�[�g���ON
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

			// �L�[�v���X����ۑ�
			state[nCnKey].press = newKey[nCnKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool Input::Press(int nKey)
{
	return (state[nKey].press & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool Input::Trigger(int nKey)
{
	return (state[nKey].trigger & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool Input::Repeat(int nKey)
{
	return (state[nKey].repeat & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
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
