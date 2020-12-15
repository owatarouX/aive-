#pragma once

class CSword
{
public:
	CSword();
	~CSword();

	void Init();
	void Updata(Math::Vector2 playerPos);
	void Draw();
	void SetTexture(KdTexture* apTexture);
	const Math::Vector2 CSword::GetPos();		//���W�擾
	const Math::Vector2 CSword::GetMove();		//�ړ��ʎ擾

	//�U������
	void Slash(Math::Vector2 Pos, const int Direct);
	const bool bGetSlash();		//�t���O��Ԏ擾

	void SetScrollPos(Math::Vector2 scrPos);	//�X�N���[���ʎ擾

private:

	KdTexture*		 m_pTexture;	//�摜(�e�N�X�`��)
	Math::Vector2	 m_pos;			//���W
	Math::Vector2	 m_move;		//���W
	Math::Matrix	 m_mat;			//�s��
	bool			 m_bSlash;		//�U���t���O
	int				 m_slashCnt;	//�U����������
	int				 m_direction;	//����
	Math::Vector2	 m_scrollPos;	//�X�N���[���ʎ擾�p

};
