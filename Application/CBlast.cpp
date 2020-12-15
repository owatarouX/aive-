#include "CBlast.h"

CBlast::CBlast()
	:m_pTexture(nullptr)
	, m_pos(0.0f, 0.0f)
	, m_mat()
	, m_bAlive(false)
	, m_aTimer(5)
	, m_scrollPos(0,0)
{
}

CBlast::~CBlast()
{
}

//初期化
void CBlast::Init()
{
	m_pos = { 0,0 };

	m_bAlive = false;

	m_scrollPos = { 0,0 };
}

//更新
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
	m_mat = DirectX::XMMatrixTranslation(m_pos.x - m_scrollPos.x, m_pos.y - m_scrollPos.y, 0.0f);
}

//描画
void CBlast::Draw()
{
	if (!m_bAlive) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(m_aCnt / m_aTimer * 96, 0, 96, 96), 1.0f);
}

//フラグ取得
const bool CBlast::IsAlive()
{
	return m_bAlive;
}

//テクスチャ設定
void CBlast::SetTexture(KdTexture* apTexture)
{
	//ポインタのアドレスが正常か調べる(nullならセットしない)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;
}

//フラグ状態設定
void CBlast::SetAlive(const float bAlive)
{
	m_bAlive = bAlive;
}

//爆発発生関数
void CBlast::Blast(const Math::Vector2 aBombPos)
{
	//爆風の生存フラグON
	m_bAlive = true;

	//爆風発生位置
	m_pos = aBombPos;
}

//スクロール量取得
void CBlast::SetScrollPos(Math::Vector2 scrPos)
{
	m_scrollPos = scrPos;
}

//座標取得
Math::Vector2 CBlast::GetPos()
{
	return m_pos;
}
