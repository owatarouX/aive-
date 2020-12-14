#pragma once

class Scene;

class CBullet
{
public:
	CBullet();
	~CBullet();

	void Init();
	void Updata();
	void Draw();
	const bool IsAlive();
	void SetTexture(KdTexture* apTexture);
	void SetAlive(const float bAlive);			//ƒtƒ‰ƒOó‘Ôİ’è
	void Shot(const Math::Vector2 aShotPos ,const int aShotDirection);
	const Math::Vector2 CBullet::GetPos();		//À•Wæ“¾
	const Math::Vector2 CBullet::GetMove();		//ˆÚ“®—Êæ“¾
	const float CBullet::GetRadius();			//”¼Œaæ“¾

	void SetScrollPos(Math::Vector2 scrPos);

private:
	KdTexture* m_pTexture;		//‰æ‘œ
	Math::Vector2 m_pos;
	Math::Vector2 m_moveVal;
	int m_moveRad;
	Math::Matrix m_mat;
	bool m_bAlive;
	const float m_radius;
	Math::Vector2	 m_scrollPos;

};