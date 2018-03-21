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
			this->speed = front*0.1f; // �X�s�[�h

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

	// ���K��
	D3DXMatrixIdentity(&mtxWorld);

	//pos = { 2, 2, 0 };
	// �����y�������@�ʒu = ��^������

	this->pos += this->speed;

	// �ǂƂ̓�����i���j
	if (this->pos.x < -24.5f || 24.5f< this->pos.x ||
		this->pos.z < -24.5f || 24.5f< this->pos.z)
	{
		//D3DXVECTOR3 p = this->pos;
		this->Destroy();
		new Effect(this->pos);
	}

	// �g��k��
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 0.3f, 0.3f, 0.3f);

	// �ړ�
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	mtxWorld *= scale*trans;


	// �G�Ƃ̓����蔻��
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (Enemy::Get(i) && this)
		{
			bool bHit = Conflict::Circle(Enemy::Get(i)->pos, BULLET_SIZE, this->pos, 1.0f);

			if (bHit)
			{
				new Effect(this->pos);		// �G�t�F�N�g
				Enemy::Get(i)->Destroy();	// �G�̏�����
				this->Destroy();			// �e��������
				Score::Inst()->Plus(10);	// �_���𑫂�
			}
		}
	}


}
void Bullet::Draw()
{
	// �`��i�ύX�E�ǉ��j�o�b�t�@�ǉ�
	Cube::Inst()->Draw(TEX_FIELD2,mtxWorld);
}