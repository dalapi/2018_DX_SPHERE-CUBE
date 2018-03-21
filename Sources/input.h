//=============================================================================
//
// ���͏��� [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

#include "GUI.h"
//
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔
#define	LIMIT_REPEAT	(30)	// ���s�[�g�J�E���g���~�b�^�[

class Input : public GUI
{
	typedef struct
	{
		BYTE press;			// �L�[�{�[�h�̓��͏�񃏁[�N
		BYTE trigger;	// �L�[�{�[�h�̃g���K�[��񃏁[�N
		BYTE release;	// �L�[�{�[�h�̃����[�X��񃏁[�N
		BYTE repeat;		// �L�[�{�[�h�̃��s�[�g��񃏁[�N
		int  repeatCnt;	// �L�[�{�[�h�̃��s�[�g�J�E���^
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

	// �V���O���g��
	static Input* Inst() 
	{
		if (instance == nullptr) { instance = new Input; }
		return instance;
	}

private:
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	void UninitInput(void);

	LPDIRECTINPUT8			g_pInput = NULL;					// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

	KEY_STATE state[NUM_KEY_MAX];

	static Input* instance;
};

#endif