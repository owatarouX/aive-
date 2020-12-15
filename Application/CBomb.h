#pragma once

#include"CBlast.h"

class CBomb
{
public:
	CBomb();
	~CBomb();

	void Init();
	void Updata();
	void Draw();
	const bool IsAlive();
	const bool GetBlastAlive();	//�t���O��Ԏ擾
	void SetTexture(KdTexture* apTexture);
	void SetBlastTexture(KdTexture* apTexture);
	void SetAlive(const float bAlive);			//�t���O��Ԑݒ�
	void InstBomb(const Math::Vector2 aBombPos);
	const Math::Vector2 CBomb::GetPos();		//���W�擾
	Math::Vector2 GetBlastPos();

	void SetScrollPos(Math::Vector2 scrPos);	//�X�N���[���ʎ擾

private:
	KdTexture* m_pTexture;		//�摜
	Math::Vector2 m_pos;
	Math::Matrix m_mat;
	bool m_bAlive;
	int	 m_aCnt;
	int m_aTimer;
	Math::Vector2 m_scrollPos;

	CBlast m_blastList;		//�����N���X
};
