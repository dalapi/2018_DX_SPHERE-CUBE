#include "Player.h"
#include "xmodel.h"
#include "Shadow.h"
#include "input.h"
#include "Controller.h"
#include "Bullet.h"
#include "BlockStage.h"
#include "SceneManager.h"
#include "Score.h"
#include "sound.h"

#include "Effects.h"

Player* Player::instance = nullptr;

#define MOVE_SPEED (0.02f) // �v���C���[�̈ړ����x
#define PLAYER_SIZE (1.2f)

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	pos = { 0,3,-17 };
	speed = { 0,0,0 };
	rot = { 0,0,0 };
	front = { 0, 0, 0 };
	fGravity = 0.3f;
	fJumpValue = 0.0f;
	bJump = bStart = bScore = bGoal = false;
	nLife = 3;
	nBlockNo = 0;

	// �ŏ��̃����_���l
	nCountFrame = nTimeCreate = 0;
	SetCreateTime();

	// AABB�ݒ�
	D3DXVECTOR3 min = D3DXVECTOR3(pos.x - ((PLAYER_SIZE - 0.6f) / 2), pos.y - (PLAYER_SIZE / 2), pos.z - ((PLAYER_SIZE - 0.6f) / 2));
	D3DXVECTOR3 max = D3DXVECTOR3(pos.x + ((PLAYER_SIZE - 0.6f) / 2), pos.y + (PLAYER_SIZE / 2), pos.z + ((PLAYER_SIZE - 0.6f) / 2));
	player = AABB(min, max, pos);
}

void Player::Uninit()
{
	Shadow::Inst()->Uninit();
	if (instance != nullptr) 
	{ 
		delete instance; 
		instance = nullptr;
	}
}

void Player::Update()
{
	// 180�x��]����
	D3DXMatrixRotationY(&m_mtxWorld, 3.14f);

	// �R���g���[���[�ł̈ړ�
	if (Input::Inst()->Press(DIK_LEFT) || Controller::Inst()->LeftStick().x < -CTRL_AXIS_RANGE * 0.5f) { speed.x -= MOVE_SPEED; }
	if (Input::Inst()->Press(DIK_RIGHT) || Controller::Inst()->LeftStick().x > CTRL_AXIS_RANGE * 0.5f) { speed.x += MOVE_SPEED; }
	if (Input::Inst()->Press(DIK_DOWN) || Controller::Inst()->LeftStick().y > CTRL_AXIS_RANGE * 0.5f) { speed.z -= MOVE_SPEED; }
	if (Input::Inst()->Press(DIK_UP) || Controller::Inst()->LeftStick().y < -CTRL_AXIS_RANGE * 0.5f) { speed.z += MOVE_SPEED; }

	if (Input::Inst()->Press(DIK_LSHIFT)) { rot.y -= 0.05f; }
	if (Input::Inst()->Press(DIK_RSHIFT)) { rot.y += 0.05f; }

	if (Input::Inst()->Repeat(DIK_SPACE) || Controller::Inst()->Repeat(CTRL_MARU)) { new Bullet(pos, front); } // �e
	if (Input::Inst()->Trigger(DIK_X) || Controller::Inst()->Trigger(CTRL_BATSU)) {
		Effects::Inst()->Play(AWA, pos);
		bJump = true; fJumpValue = 0.8f; }  // ��

	D3DXVECTOR3 speedNor;
	D3DXVec3Normalize(&speedNor, &speed);
	D3DXVECTOR3 frontNor;
	D3DXVec3Normalize(&frontNor, &front);

	// ���ȏ�̑��x������ꍇ
	if (speed.x < -0.05f || speed.x > 0.05f ||
		speed.z < -0.05f || speed.z > 0.05f)
	{
		// �O��
		D3DXVECTOR3 up;
		// .y��1or-1���o��
		D3DXVec3Cross(&up, &frontNor, &speedNor);

		if ((int)up.y == 0)
		{
			// �O�x�N�g���Ƒ��x�x�N�g���̓���(-1~1)
			if (D3DXVec3Dot(&frontNor, &speedNor) < 0.0f) { up.y = 1; }
		}

		// ��]
		float temp = 0.1f;
		rot.y += temp * up.y;
	}
	
	pos += speed;
	speed *= 0.9f;
	player.update(pos); // AABB�X�V
	bFloor = 0;

	// �n�ʔ���
	for (int n = 0; n < MAX_BLOCK; n++)
	{
		AABB blockab = AABB(BlockStage::Inst()->GetMin(n), BlockStage::Inst()->GetMax(n), BlockStage::Inst()->GetPos(n));
		bool bHit = Conflict::AABBs(player, blockab);
		if (bHit) { 
			bFloor = true;
			nBlockNo = n;
			if (nBlockNo == TARGET)
			{
				Score::Inst()->Plus(100 + rand() % 20);
				Effects::Inst()->Play(DEAD, pos);
				bGoal = 1;
			}
			BlockStage::Inst()->SetTex(C_GRAY,n); 
		}
	}

	// ��
	if (bJump && 11.0f > pos.y)
	{
		pos.y += fJumpValue;
		fJumpValue *= 0.9f;
	}
	if (fJumpValue == 0) { bJump = 0; }
	if (!bFloor) { pos.y -= fGravity*0.8; }
	
	// �X�^�[�g����
	if (pos.z > -10.0f) { bStart = true; Sound::Inst()->Play(SE_WAIT); }

	D3DXMATRIX m_trans;
	D3DXMatrixTranslation(&m_trans, pos.x, pos.y, pos.z);
	D3DXMATRIX m_rorate;
	D3DXMatrixRotationYawPitchRoll(&m_rorate, rot.y, rot.x, rot.z);
	D3DXMATRIX m_scale;
	D3DXMatrixScaling(&m_scale, 1.0f, 1.0f, 1.0f);

	m_mtxWorld *= m_rorate *m_scale *m_trans;

	// �����_����
	D3DXMatrixTranslation(&m_mtxSon, 0, 0, -5);
	m_mtxSon *= m_mtxWorld;
	D3DXVECTOR3 SonPos = { m_mtxSon._41, m_mtxSon._42, m_mtxSon._43 };

	// �O�����v�Z
	front = SonPos - pos;

	// ��
	if (pos.y - PLAYER_SIZE < -15) { 
		if (bStart) {Dead(); }		// �X�^�[�g
		else if(!bStart) { pos = { 0,3,-17 }; }	// �����ʒu
		}

	// �ړ��͈�
	/*
	if (pos.x - PLAYER_SIZE < -25 || 25 < pos.x + PLAYER_SIZE){
		pos.x -= speed.x;
		speed.x = 0;
	}
	if (pos.z - PLAYER_SIZE < -25 || 25 < pos.z + PLAYER_SIZE){
		pos.z -= speed.z;
		speed.z = 0;
	}
	*/

	Shadow::Inst()->Update();
	if (Input::Inst()->Trigger(DIK_N)) { Effects::Inst()->Play(SQUE, pos); }
	Effects::Inst()->Update();


	nCountFrame++;

	if (nCountFrame == nTimeCreate)
	{
		Effects::Inst()->Play(SQUE, pos);
		Effects::Inst()->Play(SORA, pos);
		nCountFrame = 0;
		SetCreateTime();
	}
}

void Player::Draw()
{
	Model::Inst()->Draw((Model::ENUM)0, m_mtxWorld,1);
	Shadow::Inst()->Draw();
}

void Player::Dead()
{
	// ����
	if (nLife > 0){
		pos = { 0,3,-17 };	// �����ʒu
		nLife -= 1;
	}
	else if(nLife <=0) { 
		Effects::Inst()->Play(PART, pos);
		//SceneManager::Inst()->Fade(GAME); // ���U���g��ʂ�
	}	
}

void Player::GUIUpdate()
{
	ImGui::Text("Player");
	ImGui::Text("pos x %.2f y %.2f z %.2f", pos.x, pos.y, pos.z);
	ImGui::Text("Jump? %d",bJump);
	ImGui::Text(" Hit? %d", bFloor);
	ImGui::Text("Life? %d", nLife);
	ImGui::Text("Score?%d", bScore);
	ImGui::Text("Count %2d Time %2d", nCountFrame, nTimeCreate);

	//ImGui::Text("Block Pos[0].y %f", BlockStage::Inst()->GetPos(0).y);
}
