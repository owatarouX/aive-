#include "CBomb.h"

CBomb::CBomb()
	:m_pTexture(nullptr)
	, m_pos(0.0f, 0.0f)
	, m_mat()
	, m_bAlive(false)
	,m_aTimer(5)
{
}

CBomb::~CBomb()
{
}

//������
void CBomb::Init()
{
	m_pos = { 0,0 };

	m_bAlive = false;

	m_scrollPos = { 0,0 };

	//����������
	m_blastList.Init();
}

//�X�V
void CBomb::Updata()
{
	
	if (m_bAlive)
	{
		const int CNT_MAX = m_aTimer * 18;
		if (m_aCnt >= CNT_MAX)
		{
			m_blastList.Blast(m_pos);
			m_bAlive = false;
			m_aCnt = 0;
		}
		m_aCnt++;

		//�s��쐬
		//m_mat = DirectX::XMMatrixTranslation(m_pos.x , m_pos.y, 0.0f);
		m_mat = DirectX::XMMatrixTranslation(m_pos.x - m_scrollPos.x, m_pos.y - m_scrollPos.y, 0.0f);
	}

	//�����X�V����
	m_blastList.SetScrollPos(m_scrollPos);
	m_blastList.Updata();

}

//�`��
void CBomb::Draw()
{
	if (m_bAlive)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(m_aCnt / m_aTimer * 64, 0, 64, 64), 1.0f);
	}

	//�����`��
	m_blastList.Draw();
}

//�t���O�擾
const bool CBomb::IsAlive()
{
	return m_bAlive;
}

//�e�N�X�`���ݒ�F���e
void CBomb::SetTexture(KdTexture* apTexture)
{
	//�|�C���^�̃A�h���X�����킩���ׂ�(null�Ȃ�Z�b�g���Ȃ�)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;
}

//�e�N�X�`���ݒ�F����
void CBomb::SetBlastTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_blastList.SetTexture(apTexture);
}

//�t���O�ݒ�
void CBomb::SetAlive(const float bAlive)
{
	m_bAlive = bAlive;
}

//�����̃t���O�擾
const bool CBomb::GetBlastAlive()
{
	return m_blastList.IsAlive();
}

//���e�����֐�
void CBomb::InstBomb(const Math::Vector2 aBombPos)
{
	//�����t���OON
	m_bAlive = true;

	//�ݒu�ʒu
	m_pos = aBombPos;
}

//���e���W�擾
const Math::Vector2 CBomb::GetPos()
{
	return Math::Vector2();

}

//�������W�擾
Math::Vector2 CBomb::GetBlastPos()
{
	return m_blastList.GetPos();
}

//�X�N���[���ʎ擾
void CBomb::SetScrollPos(Math::Vector2 scrPos)
{
	m_scrollPos = scrPos;
}

