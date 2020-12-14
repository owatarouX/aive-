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

	//指定角度に移動
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
	

	//移動行列
	m_mat = DirectX::XMMatrixTranslation(m_pos.x-m_scrollPos.x, m_pos.y - m_scrollPos.y, 0.0f);


}

//描画
void CBullet::Draw()
{
	if (!m_bAlive) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(0, 0, 16, 16), 1.0f);
}

//生存フラグ取得
const bool CBullet::IsAlive()
{
	return m_bAlive;
}

//テクスチャ設定
void CBullet::SetTexture(KdTexture* apTexture)
{
	//ポインタのアドレスが正常か調べる(nullならセットしない)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;
}

//フラグ状態取得
void CBullet::SetAlive(const float bAlive)
{
	m_bAlive = bAlive;
}

//発射処理
void CBullet::Shot(const Math::Vector2 aShotPos, const int aShotDirection)
{
	//生存フラグON
	m_bAlive = true;

	//発射位置
	m_pos = aShotPos;

	//発射角度
	m_moveRad = aShotDirection;
}

const Math::Vector2 CBullet::GetPos()
{
	return m_pos;
}

//移動量取得
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


