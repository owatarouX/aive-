#pragma once

class Scene;

class CBullet
{
public:
	CBullet();
	~CBullet();

	void Init();
	void Updata();
	void Draw();
	const bool IsAlive();
	void SetTexture(KdTexture* apTexture);
	void SetAlive(const float bAlive);			//�t���O��Ԑݒ�
	void SetOwner(Scene* apOwner);
	void Shot(const Math::Vector2 aShotPos ,const int aShotDirection);
	const Math::Vector2 CBullet::GetPos();		//���W�擾
	const Math::Vector2 CBullet::GetMove();		//�ړ��ʎ擾
	const float CBullet::GetRadius();			//���a�擾


private:
	KdTexture* m_pTexture;		//�摜
	Math::Vector2 m_pos;
	Math::Vector2 m_moveVal;
	int m_moveRad;
	Math::Matrix m_mat;
	bool m_bAlive;
	const float m_radius;
	
	Scene* m_pOwner;	// �I�[�i�[�擾�p
};