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
	const bool GetBlastAlive();	//フラグ状態取得
	void SetTexture(KdTexture* apTexture);
	void SetBlastTexture(KdTexture* apTexture);
	void SetAlive(const float bAlive);			//フラグ状態設定
	void InstBomb(const Math::Vector2 aBombPos);
	const Math::Vector2 CBomb::GetPos();		//座標取得
	Math::Vector2 GetBlastPos();

	void SetScrollPos(Math::Vector2 scrPos);	//スクロール量取得

private:
	KdTexture* m_pTexture;		//画像
	Math::Vector2 m_pos;
	Math::Matrix m_mat;
	bool m_bAlive;
	int	 m_aCnt;
	int m_aTimer;
	Math::Vector2 m_scrollPos;

	CBlast m_blastList;		//爆発クラス
};
