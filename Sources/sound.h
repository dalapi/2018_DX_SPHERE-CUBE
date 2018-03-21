//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <xaudio2.h>

//*****************************************************************************
// サウンドファイル
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
// プロトタイプ宣言
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

	IXAudio2 *g_pXAudio2 = nullptr;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *g_pMasteringVoice = nullptr;			// マスターボイス
	IXAudio2SourceVoice *g_apSourceVoice[SOUND_MAX] = {};	// ソースボイス
	BYTE *g_apDataAudio[SOUND_MAX] = {};					// オーディオデータ
	DWORD g_aSizeAudio[SOUND_MAX] = {};					// オーディオデータサイズ

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

};

#endif
