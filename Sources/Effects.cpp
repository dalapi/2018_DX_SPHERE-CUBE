#include "Effects.h"
#include "Camera.h"
#include "sound.h"

Effects* Effects::instance = nullptr;

void Effects::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 描画用インスタンスの生成
	g_renderer = ::EffekseerRendererDX9::Renderer::Create(pDevice, 2000);

	// エフェクト管理用インスタンスの生成
	g_manager = ::Effekseer::Manager::Create(2000);

	// 描画用インスタンスから描画機能を設定
	g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
	g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
	g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
	g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());
	g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
	g_manager->SetModelLoader(g_renderer->CreateModelLoader());

	// 音再生用インスタンスの生成
	g_sound = ::EffekseerSound::Sound::Create(Sound::Inst()->GetXA2(), 16, 16);

	// 音再生用インスタンスから再生機能を指定
	g_manager->SetSoundPlayer(g_sound->CreateSoundPlayer());

	// 音再生用インスタンスからサウンドデータの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	g_manager->SetSoundLoader(g_sound->CreateSoundLoader());

	// 視点位置を確定
	g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// 投影行列を設定
	g_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovRH(D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f));

	// カメラ行列を設定
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}

void Effects::Uninit()
{
	// エフェクトの停止
	g_manager->StopEffect(g_handle);

	// エフェクトの破棄
	ES_SAFE_RELEASE(g_effect);

	// 先にエフェクト管理用インスタンスを破棄
	g_manager->Destroy();

	// 次に音再生用インスタンスを破棄
	g_sound->Destroy();

	// 次に描画用インスタンスを破棄
	g_renderer->Destroy();

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Effects::Play(EFFECT_ENUM num,D3DXVECTOR3 pos)
{
	// エフェクトの読込
	g_effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*)g_Effects[num]);

	// エフェクトの再生
	g_handle = g_manager->Play(g_effect, pos.x, pos.y, pos.z);
}

void Effects::Stop()
{
	// エフェクトの停止
	g_manager->StopEffect(g_handle);

	// エフェクトの破棄
	ES_SAFE_RELEASE(g_effect);
}

void Effects::Update()
{
	// 視点位置を確定
	g_position = ::Effekseer::Vector3D(Camera::Inst()->GetEye().x, Camera::Inst()->GetEye().y, Camera::Inst()->GetEye().z);

	// カメラ行列を設定
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(
			g_position,
			::Effekseer::Vector3D(Camera::Inst()->GetAt().x, Camera::Inst()->GetAt().y, Camera::Inst()->GetAt().z),
			::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	// エフェクトの移動処理を行う
	g_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f));

	// エフェクトの更新処理を行う
	g_manager->Update();
}

void Effects::Draw()
{
	// エフェクトの描画を行う。
	g_manager->Draw();
}