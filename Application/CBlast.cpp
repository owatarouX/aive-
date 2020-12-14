#include "CBlast.h"

CBlast::CBlast()
	:m_pTexture(nullptr)
	, m_pos(0.0f, 0.0f)
	, m_mat()
	, m_bAlive(false)
	, m_aTimer(5)
{
}

CBlast::~CBlast()
{
}

void CBlast::Init()
{
	m_pos = { 0,0 };

	m_bAlive = false;
}

void CBlast::Updata()
{
	if (!m_bAlive) return;
	const int CNT_MAX = m_aTimer * 5;
	if (m_aCnt >= CNT_MAX)
	{
		m_bAlive = false;
		m_aCnt = 0;
	}
	m_aCnt++;

	//行列作成
	m_mat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, 0.0f);
}

void CBlast::Draw()
{
	if (!m_bAlive) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(m_aCnt / m_aTimer * 96, 0, 96, 96), 1.0f);
}

const bool CBlast::IsAlive()
{
	return m_bAlive;
}

void CBlast::SetTexture(KdTexture* apTexture)
{
	//ポインタのアドレスが正常か調べる(nullならセットしない)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;
}

void CBlast::SetAlive(const float bAlive)
{
	m_bAlive = bAlive;
}

void CBlast::Blast(const Math::Vector2 aBombPos)
{
	//爆風の生存フラグON
	m_bAlive = true;

	//爆風発生位置
	m_pos = aBombPos;
}
