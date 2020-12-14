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

void CBomb::Init()
{
	m_pos = { 0,0 };

	m_bAlive = false;

}

void CBomb::Updata()
{
	if (!m_bAlive) return;
	const int CNT_MAX =m_aTimer*18;
	if (m_aCnt >= CNT_MAX)
	{
		m_bAlive = false;

		m_aCnt = 0;
	}
	m_aCnt++;

	//行列作成
	m_mat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, 0.0f);
}

void CBomb::Draw()
{
	if (!m_bAlive) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(m_aCnt/m_aTimer*64, 0, 64, 64), 1.0f);
}

const bool CBomb::IsAlive()
{
	return m_bAlive;
}

void CBomb::SetTexture(KdTexture* apTexture)
{
	//ポインタのアドレスが正常か調べる(nullならセットしない)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;
}

void CBomb::SetAlive(const float bAlive)
{
	m_bAlive = bAlive;
}


void CBomb::InstBomb(const Math::Vector2 aBombPos)
{
	//生存フラグON
	m_bAlive = true;

	//設置位置
	m_pos = aBombPos;
}

const Math::Vector2 CBomb::GetPos()
{
	return Math::Vector2();
}
