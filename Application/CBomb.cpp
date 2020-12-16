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

//初期化
void CBomb::Init()
{
	m_pos = { 0,0 };

	m_bAlive = false;

	m_scrollPos = { 0,0 };

	//爆発初期化
	m_blastList.Init();
}

//更新
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

		//行列作成
		//m_mat = DirectX::XMMatrixTranslation(m_pos.x , m_pos.y, 0.0f);
		m_mat = DirectX::XMMatrixTranslation(m_pos.x - m_scrollPos.x, m_pos.y - m_scrollPos.y, 0.0f);
	}

	//爆発更新処理
	m_blastList.SetScrollPos(m_scrollPos);
	m_blastList.Updata();

}

//描画
void CBomb::Draw()
{
	if (m_bAlive)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(m_aCnt / m_aTimer * 64, 0, 64, 64), 1.0f);
	}

	//爆発描画
	m_blastList.Draw();
}

//フラグ取得
const bool CBomb::IsAlive()
{
	return m_bAlive;
}

//テクスチャ設定：爆弾
void CBomb::SetTexture(KdTexture* apTexture)
{
	//ポインタのアドレスが正常か調べる(nullならセットしない)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;
}

//テクスチャ設定：爆発
void CBomb::SetBlastTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_blastList.SetTexture(apTexture);
}

//フラグ設定
void CBomb::SetAlive(const float bAlive)
{
	m_bAlive = bAlive;
}

//爆発のフラグ取得
const bool CBomb::GetBlastAlive()
{
	return m_blastList.IsAlive();
}

//爆弾発生関数
void CBomb::InstBomb(const Math::Vector2 aBombPos)
{
	//生存フラグON
	m_bAlive = true;

	//設置位置
	m_pos = aBombPos;
}

//爆弾座標取得
const Math::Vector2 CBomb::GetPos()
{
	return Math::Vector2();

}

//爆発座標取得
Math::Vector2 CBomb::GetBlastPos()
{
	return m_blastList.GetPos();
}

//スクロール量取得
void CBomb::SetScrollPos(Math::Vector2 scrPos)
{
	m_scrollPos = scrPos;
}

