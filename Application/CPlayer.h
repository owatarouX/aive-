#pragma once

#include"CBullet.h"
#include"CSword.h"
#include"CBomb.h"

//クラスの前方宣言
class Scene;

enum eDirection	//向いている方向
{
	Up,
	Down,
	Left,
	Right
};

//武器セレクト
enum eClick
{
	eShuriken,	//0.手裏剣
	eSword,		//1.刀
	eBomb		//2.爆弾
};

//手裏剣の要素数
constexpr int BULLET_MAX = 2;

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

	//セッター
	void SetTexture(KdTexture* apTexture);
	void SetBulletTexture(KdTexture* apTexture);
	void SetSlashTexture(KdTexture* apTexture);
	void SetBombTexture(KdTexture* apTexture);
	void SetBlastTexture(KdTexture* apTexture);
	void SetOwner(Scene* apOwner);

	//ゲッター
	const Math::Vector2 GetPos();		//プレイヤー座標取得
	const int GetHp();
	const int GetR();
	const int GetL();

private:		//外部からアクセス不可

	eDirection		m_direction;
	eClick			m_LClick;
	eClick			m_RClick;

	KdTexture* m_pTexture;//画像(テクスチャ)
	Math::Vector2	 m_pos;		//座標
	Math::Vector2	 m_moveVal;	//移動量
	Math::Matrix	 m_mat;		//行列
	Scene* m_pOwner;	//オーナー取得用
	
	bool			 m_bAlive;  //生存フラグ
	int				 m_hp;		//HP
	int				 m_hpCount;	//無敵時間
	float			 m_alpha;	//画像の透明度
	bool			 m_HitFlg;	//敵と当たったか判定
	
	bool			 m_bRClick;	//右クリックフラグ
	bool			 m_bLClick;	//左クリックフラグ
	bool			 m_bRChange;//右武器変更フラグ
	bool			 m_bLChange;//左武器変更フラグ

	int	 m_aCnt	;	//アニメーションカウント
	int m_aTimer;	//アニメーションを何fに1枚動かす
	int m_aflame;	//アニメーション枚数


	void HitCheckMap();			//マップとの当たり判定
	void HitCheckEnemy();		//敵との当たり判定
	void HitCheckBomb();		//爆弾との当たり判定
	void InviTime();			//無敵時間
	eClick ChangeItem(eClick click);		//武器変更関数

	CBullet m_bulletList[BULLET_MAX];
	void SetShuriken();
	CSword  m_swordList;
	void SetSword();
	CBomb  m_bombList;
	void SetBomb();

	int Animation(int cnt, const int xtex);
	
};