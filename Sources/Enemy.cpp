#include "Enemy.h"
#include "Cube.h"
#include "Effect.h"
#include "Player.h"
#include "Conflict.h"

CreateEnemy* CreateEnemy::instance = nullptr;
Enemy* Enemy::enemy[MAX_ENEMY];

Enemy::Enemy(TEXTURE_ENUM tex,D3DXVECTOR3 pos, D3DXVECTOR3 front, D3DXVECTOR3 size, ENEMY_TYPE type)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i] == nullptr)
		{
			enemy[i] = this;
			this->index = i;

			this->pos = pos;
			this->tex = tex;
			this->size = size;
			this->type = type;
			D3DXVec3Normalize(&front, &front);
			this->speed = front*0.05f; // スピード

			break;
		}
	}
}

Enemy::~Enemy()
{
	if (this->index < 0) { return; }
	if (Enemy::enemy[this->index] != nullptr)
	{	
		Enemy::enemy[this->index] = nullptr;
	}
}

void Enemy::Uninit()
{
}

void Enemy::Update()
{
	// 正規化
	D3DXMatrixIdentity(&mtx);

	if (this->type == FLY) { this->pos += this->speed; }
	else if (this->type == JUMP) { this->pos += this->speed; if (pos.y <0.5) { pos.y = 1; } }

	// 壁との当たり（仮）
	if (this->pos.x < -24.5f || 24.5f< this->pos.x
		|| this->pos.y < -24.5f || 24.5f< this->pos.y)
		//|| this->pos.z < -24.5f || 24.5f< this->pos.z)
	{
		this->Destroy();
		//new Effect(this->pos);
	}	
	
	// Playerとの当たり
	if (Conflict::Circle(this->pos, this->size.x, Player::Inst()->Get(), 1.0f))
	{
		this->Destroy();
		new Effect(this->pos);
		Player::Inst()->Dead();
	}

	// 拡大縮小
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, size.x, size.y, size.z);

	// 移動
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	mtx *= scale*trans;
}

void Enemy::Draw()
{
	Cube::Inst()->Draw(tex, mtx);
}


//---------------------------------

CreateEnemy::CreateEnemy()
{
	// 最初のランダム値
	nCountFrame = nTimeCreate = 0;
	SetCreateTime();
}
CreateEnemy::~CreateEnemy()
{
}

void CreateEnemy::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void CreateEnemy::Update()
{
	if (Player::Inst()->GetStart())
	{
		nCountFrame++;

		// 敵を生成する
		if (nCountFrame == nTimeCreate)
		{
			//new Enemy(C_BLACK, D3DXVECTOR3(-8, 1, 5), D3DXVECTOR3(2, 0, 0), D3DXVECTOR3(0.8, 0.5, 0.5), FLY);
			//new Enemy(C_GRAY, D3DXVECTOR3(10, 1, 0), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0.8, 0.5, 0.5), FLY);
			//new Enemy(C_GRAY, D3DXVECTOR3(-1.5, 1, 15), D3DXVECTOR3(0, -0.4, -0.5), D3DXVECTOR3(0.3, 0.3, 0.3), JUMP);
			//new Enemy(C_BLACK, D3DXVECTOR3(-10, 1, 8), D3DXVECTOR3(0.5, -0.4, 0), D3DXVECTOR3(0.3, 0.3, 0.3), JUMP);

			nCountFrame = 0;
			SetCreateTime();
		}
	}
	
}


void CreateEnemy::SetCreateTime()
{
	// 範囲内乱数生成
	nTimeCreate = rand() % (CREATE_TIME_MAX - CREATE_TIME_MIN) + CREATE_TIME_MIN;
}

void CreateEnemy::GUIUpdate()
{
	ImGui::Text("CreateEnemy");
	ImGui::Text("nCountFrame %2d nTimeCreate %2d", nCountFrame, nTimeCreate);
}