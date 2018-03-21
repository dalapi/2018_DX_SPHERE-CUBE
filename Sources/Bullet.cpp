#include "main.h"
#include "Bullet.h"
#include "input.h"
#include "Texture.h"
#include "camera.h"
#include "Effect.h"
#include "Cube.h"

#include "Conflict.h"
#include "Enemy.h"
#include "Score.h"

Bullet* Bullet::bullet[MAX_BULLET];

Bullet::Bullet(D3DXVECTOR3 pos, D3DXVECTOR3 front)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	//	HRESULT hr;

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (bullet[i] == nullptr)
		{
			bullet[i] = this;
			this->index = i;

			this->pos = pos;
			D3DXVec3Normalize(&front, &front);
			this->speed = front*0.1f; // スピード

			break;
		}
	}
}
Bullet::~Bullet()
{
	if (this->index < 0) { return; }
	if (Bullet::bullet[this->index] != nullptr)
	{
		Bullet::bullet[this->index] = nullptr;
	}
}

void Bullet::Uninit(void)
{

}

void Bullet::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if (pDevice == NULL) { return; }

	// 正規化
	D3DXMatrixIdentity(&mtxWorld);

	//pos = { 2, 2, 0 };
	// 注視點減去相機位置 = 一個運動向量

	this->pos += this->speed;

	// 壁との当たり（仮）
	if (this->pos.x < -24.5f || 24.5f< this->pos.x ||
		this->pos.z < -24.5f || 24.5f< this->pos.z)
	{
		//D3DXVECTOR3 p = this->pos;
		this->Destroy();
		new Effect(this->pos);
	}

	// 拡大縮小
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 0.3f, 0.3f, 0.3f);

	// 移動
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	mtxWorld *= scale*trans;


	// 敵との当たり判定
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (Enemy::Get(i) && this)
		{
			bool bHit = Conflict::Circle(Enemy::Get(i)->pos, BULLET_SIZE, this->pos, 1.0f);

			if (bHit)
			{
				new Effect(this->pos);		// エフェクト
				Enemy::Get(i)->Destroy();	// 敵の消える
				this->Destroy();			// 弾を消える
				Score::Inst()->Plus(10);	// 点数を足す
			}
		}
	}


}
void Bullet::Draw()
{
	// 描画（変更・追加）バッファ追加
	Cube::Inst()->Draw(TEX_FIELD2,mtxWorld);
}