#include "Camera.h"
#include "input.h"
#include "Player.h"

// �ړ�����(float)�A��]���x�i���W�A���Eframe�j
static float g_fMoveSpeed = 0.07f, g_dRadian = 0.02f;

// �Q�[���V�[���ǐՊp�x�i�v���C���[�j
#define STD_Y (3.0f)
#define STD_Z (-5.0f)

Camera* Camera::instance = nullptr;

void Camera::Init(SCENE sc)
{
	switch(sc)
	{
	case GAME:
		g_eye = { 0.0f, STD_Y, STD_Z };
		g_at = { 0.0f, 0.0f, 0.0f };
		g_up = { 0.0f, 1.0f, 0.0f };
		break;
	case TITLE:
		g_eye = { 2.0f, 3.0f, -9.0f };
		g_at = { 5.0f, 4.0f, -4.0f };
		g_up = { 0.0f, 1.0f, 0.0f };
		break;
	case RESULT:
		break;
	case TUTORIAL:
		g_eye = { -1.0f, 0.0f, -5.0f };
		g_at = { 3.0f, 0.0f, 0.0f };
		g_up = { 0.0f, 1.0f, 0.0f };
		break;
	}

	repeatAt = repeatPos = 0;
	bRepeatAt = bRepeatPos = false;
}

void Camera::Uninit(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Camera::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//--------------------------------
	if (SceneManager::Inst()->GetScene()==GAME)
	{
		// �v���C���[�ɒǐՂ���
		g_at = Player::Inst()->Get();
		g_at.y += 1.5f;

		// ����p�x��������
		g_eye = Player::Inst()->Get();
		g_eye.y += STD_Y;
		g_eye.z += STD_Z;
	}
	//--------------------------------

	// �O���������߂�
	g_front = g_at - g_eye;
	D3DXVECTOR3 vec = g_front;
	vec.y = 0.0f;

	// vec �𐳋K���i�m�[�}���C�Y�j����
	D3DXVec3Normalize(&vec, &vec);

	// �E�����x�N�g�������߂�
		// �O�ρ@�@�o�́@�@���Ӓl�@�@�E�Ӓl
	D3DXVec3Cross(&g_right, &g_front, &g_up);

	D3DXVec3Normalize(&g_up, &g_up);
	D3DXVec3Normalize(&g_right, &g_right);

	//--------------------------------

	// �x�N�g�����W�ϊ�
	D3DXVECTOR3 dir = g_at - g_eye;

	//--------------------------------

	//���_�𒆐S�Ƃ��ĉ�]����
	if (Input::Inst()->Press(DIK_X))
	{	// at eye�������Ă݂�
		D3DXVECTOR3 dir2 = g_eye - g_at;
		D3DXMatrixRotationY(&mtxRot, g_dRadian);
		D3DXVec3TransformNormal(&dir2, &dir2, &mtxRot);
		g_eye = g_at + dir2;
	}
	//--------------------------------

	// �r���[�s��
	D3DXMatrixLookAtLH(&mtxView, &g_eye, &g_at, &g_up);

	// �v���W�F�N�V�����i�p�[�X�y�N�e�B�u�j�s��̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000);

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// ���_������
	if (Input::Inst()->Press(DIK_R)){Init(sc);}
	
}

void Camera::UpdateAt(int count, POS pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_front = g_at - g_eye;	// �O���������߂�
	D3DXVECTOR3 vec = g_front;
	vec.y = 0.0f;
	D3DXVec3Normalize(&vec, &vec);	// vec �𐳋K������

	D3DXVec3Cross(&g_right, &g_front, &g_up);// �E�����x�N�g�������߂�
	D3DXVec3Normalize(&g_up, &g_up);
	D3DXVec3Normalize(&g_right, &g_right);

	// �x�N�g�����W�ϊ�
	D3DXVECTOR3 dir = g_at - g_eye;
	
	if (repeatAt < count&& bRepeatAt)
	{
		switch (pos)
		{
		case UP:
			D3DXMatrixRotationAxis(&mtxRot, &g_right, g_dRadian);
			D3DXVec3TransformNormal(&dir, &dir, &mtxRot);
			g_at = g_eye + dir;
			break;
		case DOWN:
			D3DXMatrixRotationAxis(&mtxRot, &g_right, g_dRadian*(-1.0f));
			D3DXVec3TransformNormal(&dir, &dir, &mtxRot);
			g_at = g_eye + dir;
			break;
		case LEFT:
			D3DXMatrixRotationY(&mtxRot, g_dRadian);
			D3DXVec3TransformNormal(&dir, &dir, &mtxRot);
			g_at = g_eye + dir;
			break;
		case RIGHT:
			D3DXMatrixRotationY(&mtxRot, g_dRadian*(-1.0f));
			D3DXVec3TransformNormal(&dir, &dir, &mtxRot);
			g_at = g_eye + dir;
			break;
		}
		repeatAt++;
	}
	else { bRepeatAt = false; repeatAt = 0; }

	D3DXMatrixLookAtLH(&mtxView, &g_eye, &g_at, &g_up);	// �r���[�s��
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000);	// �v���W�F�N�V�����s��

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

void Camera::UpdatePos(int count, POS pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_front = g_at - g_eye;	// �O���������߂�
	D3DXVECTOR3 vec = g_front;
	vec.y = 0.0f;
	D3DXVec3Normalize(&vec, &vec);	// vec �𐳋K������

	D3DXVec3Cross(&g_right, &g_front, &g_up);// �E�����x�N�g�������߂�
	D3DXVec3Normalize(&g_up, &g_up);
	D3DXVec3Normalize(&g_right, &g_right);

	if (repeatPos < count && bRepeatPos)
	{
		switch (pos)
		{
		case UP:
			g_eye += vec*g_fMoveSpeed;
			g_at += vec*g_fMoveSpeed;
			break;
		case DOWN:
			g_eye -= vec*g_fMoveSpeed;
			g_at -= vec*g_fMoveSpeed;
			break;
		case LEFT:
			g_eye += g_right*g_fMoveSpeed;
			g_at += g_right*g_fMoveSpeed;
			break;
		case RIGHT:
			g_eye -= g_right*g_fMoveSpeed;
			g_at -= g_right*g_fMoveSpeed;
			break;
		}
		repeatPos++;
	}
	else { bRepeatPos = false; repeatPos = 0; }

	D3DXMatrixLookAtLH(&mtxView, &g_eye, &g_at, &g_up);	// �r���[�s��
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000);	// �v���W�F�N�V�����s��

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}


void Camera::GUIUpdate()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("g_eye %.f %.f %.f", g_eye.x, g_eye.y, g_eye.z);
		ImGui::Text(" g_at %.f %.f %.f", g_at.x, g_at.y, g_at.z);
		ImGui::Text(" g_up %.f %.f %.f", g_up.x, g_up.y, g_up.z);
		ImGui::Text(" repeatAt  %d %d", repeatAt, bRepeatAt);
		ImGui::Text(" repeatPos %d %d", repeatPos, bRepeatPos);

		ImGui::End();
	}
}