#include "Camera.h"
#include "input.h"
#include "Player.h"

// 移動数字(float)、回転速度（ラジアン・frame）
static float g_fMoveSpeed = 0.07f, g_dRadian = 0.02f;

// ゲームシーン追跡角度（プレイヤー）
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
		// プレイヤーに追跡する
		g_at = Player::Inst()->Get();
		g_at.y += 1.5f;

		// 見る角度調整する
		g_eye = Player::Inst()->Get();
		g_eye.y += STD_Y;
		g_eye.z += STD_Z;
	}
	//--------------------------------

	// 前方向を求める
	g_front = g_at - g_eye;
	D3DXVECTOR3 vec = g_front;
	vec.y = 0.0f;

	// vec を正規化（ノーマライズ）する
	D3DXVec3Normalize(&vec, &vec);

	// 右方向ベクトルを求める
		// 外積　　出力　　左辺値　　右辺値
	D3DXVec3Cross(&g_right, &g_front, &g_up);

	D3DXVec3Normalize(&g_up, &g_up);
	D3DXVec3Normalize(&g_right, &g_right);

	//--------------------------------

	// ベクトル座標変換
	D3DXVECTOR3 dir = g_at - g_eye;

	//--------------------------------

	//視点を中心として回転する
	if (Input::Inst()->Press(DIK_X))
	{	// at eye交換してみる
		D3DXVECTOR3 dir2 = g_eye - g_at;
		D3DXMatrixRotationY(&mtxRot, g_dRadian);
		D3DXVec3TransformNormal(&dir2, &dir2, &mtxRot);
		g_eye = g_at + dir2;
	}
	//--------------------------------

	// ビュー行列
	D3DXMatrixLookAtLH(&mtxView, &g_eye, &g_at, &g_up);

	// プロジェクション（パースペクティブ）行列の作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000);

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// 視点初期化
	if (Input::Inst()->Press(DIK_R)){Init(sc);}
	
}

void Camera::UpdateAt(int count, POS pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_front = g_at - g_eye;	// 前方向を求める
	D3DXVECTOR3 vec = g_front;
	vec.y = 0.0f;
	D3DXVec3Normalize(&vec, &vec);	// vec を正規化する

	D3DXVec3Cross(&g_right, &g_front, &g_up);// 右方向ベクトルを求める
	D3DXVec3Normalize(&g_up, &g_up);
	D3DXVec3Normalize(&g_right, &g_right);

	// ベクトル座標変換
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

	D3DXMatrixLookAtLH(&mtxView, &g_eye, &g_at, &g_up);	// ビュー行列
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000);	// プロジェクション行列

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

void Camera::UpdatePos(int count, POS pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_front = g_at - g_eye;	// 前方向を求める
	D3DXVECTOR3 vec = g_front;
	vec.y = 0.0f;
	D3DXVec3Normalize(&vec, &vec);	// vec を正規化する

	D3DXVec3Cross(&g_right, &g_front, &g_up);// 右方向ベクトルを求める
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

	D3DXMatrixLookAtLH(&mtxView, &g_eye, &g_at, &g_up);	// ビュー行列
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DX_PI / 3, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000);	// プロジェクション行列

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