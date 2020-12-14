#pragma once

class Scene;

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
	void SetAlive(const float bAlive);			//ƒtƒ‰ƒOó‘Ôİ’è
	void Blast(const Math::Vector2 aBombPos);

private:
	KdTexture* m_pTexture;		//‰æ‘œ
	Math::Vector2 m_pos;
	Math::Matrix m_mat;
	bool m_bAlive;
	int	 m_aCnt;
	int m_aTimer;
};
