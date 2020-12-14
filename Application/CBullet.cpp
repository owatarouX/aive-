#include "CBullet.h"
#include "Utility.h"

CBullet::CBullet()
	:m_pTexture(nullptr)
	, m_pos(0.0f, 0.0f)
	, m_moveVal(0.0f, 0.0f)
	, m_moveRad(0.0f)
	, m_mat()
	, m_bAlive(false)
	, m_radius(8.0f)
{
}

CBullet::~CBullet()
{
}

void CBullet::Init()
{
	m_moveVal.x = 15.0f;
	m_moveVal.y = 15.0f;

	m_bAlive = false;
	m_scrollPos = { 0,0 };
}

void CBullet::Updata()
{
	if (!m_bAlive) return;

	//�w��p�x�Ɉړ�
	switch (m_moveRad) {
	case 0:
		m_pos.y += m_moveVal.y;
		break;
	case 1:
		m_pos.y -= m_moveVal.y;
		break;
	case 2:
		m_pos.x -= m_moveVal.x;
		break;
	case 3:
		m_pos.x += m_moveVal.x;
		break;
	}
	

	//�ړ��s��
	m_mat = DirectX::XMMatrixTranslation(m_pos.x-m_scrollPos.x, m_pos.y - m_scrollPos.y, 0.0f);


}

//�`��
void CBullet::Draw()
{
	if (!m_bAlive) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(0, 0, 16, 16), 1.0f);
}

//�����t���O�擾
const bool CBullet::IsAlive()
{
	return m_bAlive;
}

//�e�N�X�`���ݒ�
void CBullet::SetTexture(KdTexture* apTexture)
{
	//�|�C���^�̃A�h���X�����킩���ׂ�(null�Ȃ�Z�b�g���Ȃ�)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;
}

//�t���O��Ԏ擾
void CBullet::SetAlive(const float bAlive)
{
	m_bAlive = bAlive;
}

//���ˏ���
void CBullet::Shot(const Math::Vector2 aShotPos, const int aShotDirection)
{
	//�����t���OON
	m_bAlive = true;

	//���ˈʒu
	m_pos = aShotPos;

	//���ˊp�x
	m_moveRad = aShotDirection;
}

const Math::Vector2 CBullet::GetPos()
{
	return m_pos;
}

//�ړ��ʎ擾
const Math::Vector2 CBullet::GetMove()
{
	return m_moveVal;
}

const float CBullet::GetRadius()
{
	return m_radius;
}

void CBullet::SetScrollPos(Math::Vector2 scrPos)
{
	m_scrollPos = scrPos;
}


