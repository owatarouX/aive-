#pragma once

class CBlast
{
public:
	CBlast();
	~CBlast();

	void Init();
	void Updata();
	void Draw();
	const bool IsAlive();
	void SetTexture(KdTexture* apTexture);
	void SetAlive(const float bAlive);			//�t���O��Ԑݒ�
	void Blast(const Math::Vector2 aBombPos);

	void SetScrollPos(Math::Vector2 scrPos);	//�X�N���[���ʎ擾

	Math::Vector2 GetPos();

private:
	KdTexture* m_pTexture;		//�摜
	Math::Vector2 m_pos;
	Math::Matrix m_mat;
	bool m_bAlive;
	int	 m_aCnt;
	int m_aTimer;

	Math::Vector2 m_scrollPos;
};
