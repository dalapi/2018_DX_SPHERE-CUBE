#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerSoundXAudio2.h>
#include "main.h" 

static const wchar_t* g_Effects[] = {
	{ L"Data/Effect/test.efk" },
	{ L"Data/Effect/gate.efk" },
	{ L"Data/Effect/particu.efk" },
	{ L"Data/Effect/square.efk" },
	{ L"Data/Effect/awa.efk" },
	{ L"Data/Effect/drop.efk" },
	{ L"Data/Effect/universe.efk" },
	{ L"Data/Effect/bossdead.efk" },
};

enum EFFECT_ENUM {
	TEST,
	GATE,
	PART,
	SQUE,
	AWA,
	DROP,
	SORA,
	DEAD,
	MAX
};


class Effects
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Play(EFFECT_ENUM num,D3DXVECTOR3 pos);
	void Stop();

	::EffekseerRendererDX9::Renderer* GetRender() { return g_renderer; }

	static Effects* Inst() { if (instance == nullptr) { instance = new Effects; }return instance; }

private:
	static Effects* instance;

	::Effekseer::Manager*				g_manager = NULL;
	::EffekseerRendererDX9::Renderer*	g_renderer = NULL;
	::EffekseerSound::Sound*			g_sound = NULL;
	::Effekseer::Effect*				g_effect = NULL;
	::Effekseer::Handle				g_handle = -1;
	::Effekseer::Vector3D			g_position;

};

#endif