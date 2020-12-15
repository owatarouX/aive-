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
	void SetAlive(const float bAlive);			//フラグ状態設定
	void Blast(const Math::Vector2 aBombPos);

	void SetScrollPos(Math::Vector2 scrPos);	//スクロール量取得

	Math::Vector2 GetPos();

private:
	KdTexture* m_pTexture;		//画像
	Math::Vector2 m_pos;
	Math::Matrix m_mat;
	bool m_bAlive;
	int	 m_aCnt;
	int m_aTimer;

	Math::Vector2 m_scrollPos;
};
