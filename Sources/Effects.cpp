#include "Effects.h"
#include "Camera.h"
#include "sound.h"

Effects* Effects::instance = nullptr;

void Effects::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// �`��p�C���X�^���X�̐���
	g_renderer = ::EffekseerRendererDX9::Renderer::Create(pDevice, 2000);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	g_manager = ::Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
	g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
	g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
	g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());
	g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
	g_manager->SetModelLoader(g_renderer->CreateModelLoader());

	// ���Đ��p�C���X�^���X�̐���
	g_sound = ::EffekseerSound::Sound::Create(Sound::Inst()->GetXA2(), 16, 16);

	// ���Đ��p�C���X�^���X����Đ��@�\���w��
	g_manager->SetSoundPlayer(g_sound->CreateSoundPlayer());

	// ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	g_manager->SetSoundLoader(g_sound->CreateSoundLoader());

	// ���_�ʒu���m��
	g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// ���e�s���ݒ�
	g_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovRH(D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f));

	// �J�����s���ݒ�
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}

void Effects::Uninit()
{
	// �G�t�F�N�g�̒�~
	g_manager->StopEffect(g_handle);

	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE(g_effect);

	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	g_manager->Destroy();

	// ���ɉ��Đ��p�C���X�^���X��j��
	g_sound->Destroy();

	// ���ɕ`��p�C���X�^���X��j��
	g_renderer->Destroy();

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Effects::Play(EFFECT_ENUM num,D3DXVECTOR3 pos)
{
	// �G�t�F�N�g�̓Ǎ�
	g_effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*)g_Effects[num]);

	// �G�t�F�N�g�̍Đ�
	g_handle = g_manager->Play(g_effect, pos.x, pos.y, pos.z);
}

void Effects::Stop()
{
	// �G�t�F�N�g�̒�~
	g_manager->StopEffect(g_handle);

	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE(g_effect);
}

void Effects::Update()
{
	// ���_�ʒu���m��
	g_position = ::Effekseer::Vector3D(Camera::Inst()->GetEye().x, Camera::Inst()->GetEye().y, Camera::Inst()->GetEye().z);

	// �J�����s���ݒ�
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(
			g_position,
			::Effekseer::Vector3D(Camera::Inst()->GetAt().x, Camera::Inst()->GetAt().y, Camera::Inst()->GetAt().z),
			::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	// �G�t�F�N�g�̈ړ��������s��
	g_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f));

	// �G�t�F�N�g�̍X�V�������s��
	g_manager->Update();
}

void Effects::Draw()
{
	// �G�t�F�N�g�̕`����s���B
	g_manager->Draw();
}