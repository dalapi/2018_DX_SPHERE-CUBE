//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <xaudio2.h>

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	BGM_TITLE = 0,		// BGM0
	BGM_GAME,			// BGM1
	BGM_OTHER,			// BGM2
	SE_KAMI,
	SE_CLICK,
	SE_CAT,
	SE_GET,
	SE_WAIT,
	SE_TODO,
	SE_DD,
	SE_CHECK,
	SOUND_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

class Sound 
{
public:
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

	IXAudio2 *GetXA2() { return g_pXAudio2; }

	static Sound* Inst() { if (instance == nullptr) { instance = new Sound; }return instance; }
private:
	static Sound* instance;

	IXAudio2 *g_pXAudio2 = nullptr;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *g_pMasteringVoice = nullptr;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *g_apSourceVoice[SOUND_MAX] = {};	// �\�[�X�{�C�X
	BYTE *g_apDataAudio[SOUND_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD g_aSizeAudio[SOUND_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

};

#endif
