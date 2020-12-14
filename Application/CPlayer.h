#pragma once

#include"CBullet.h"

//クラスの前方宣言
class Scene;

enum eDirection	//向いている方向
{
	Up,
	Down,
	Left,
	Right
};

//手裏剣の要素数
constexpr int BULLET_MAX = 20;

//プレイヤークラス
class CPlayer
{
public:

	CPlayer();	//コンストラクタ
	~CPlayer();	//デストラクタ

	//プレイヤー
	void Init();
	void Updata();
	void Draw();
	const bool IsAlive();
	void SetTexture(KdTexture* apTexture);
	void SetBulletTexture(KdTexture* apTexture);
	void SetOwner(Scene* apOwner);
	int SetScrollDirect();	//スクロール方向取得

	const Math::Vector2 GetPos();		//プレイヤー座標取得
	const int GetHp();

	

private:		//外部からアクセス不可

	eDirection		direction;

	KdTexture*		 m_pTexture;//画像(テクスチャ)
	Math::Vector2	 m_pos;		//座標
	Math::Vector2	 m_moveVal;	//移動量
	Math::Matrix	 m_mat;		//行列
	bool			 m_bAlive;  //生存フラグ
	Scene*			 m_pOwner;	//オーナー取得用
	int	m_hp;					//HP
	int m_hpCount;				//無敵時間
	float m_alpha;				//画像の透明度
	
	void HitCheckMap();		//マップとの当たり判定
	void HitCheckEnemy();	//敵との当たり判定
	void InviTime();			//無敵時間

	CBullet m_bulletList[BULLET_MAX];
};