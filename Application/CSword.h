#pragma once

class CSword
{
public:
	CSword();
	~CSword();

	void Init();
	void Updata();
	void Draw();
	void SetTexture(KdTexture* apTexture);
	
	//攻撃処理
	void Slash(Math::Vector2 Pos, const int Direct);
	const bool bGetSlash();		//フラグ状態取得

private:

	KdTexture*		 m_pTexture;	//画像(テクスチャ)
	Math::Vector2	 m_pos;			//座標
	Math::Vector2	 m_move;		//座標
	Math::Matrix	 m_mat;			//行列
	bool			 m_bSlash;		//攻撃フラグ
	int				 m_slashCnt;	//攻撃発動時間
	int				 m_direction;	//方向
};
