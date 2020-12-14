#pragma once

//クラスの前方宣言
class Scene;

class CEnemy
{
public:		// 外部からアクセス可
	CEnemy();		// コンストラクタ
	~CEnemy();		// デストラクタ

	void Init();			// 初期化
	void Update();			// 更新
	void Draw();			// 描画
	const bool IsAlive();	// 生存確認
	void SetTexture(KdTexture* apTexture);	// テクスチャ受け取り
	Math::Vector2 GetPos();	// 座標取得
	void Moveleftandright();// 左右移動 
	void Homing();			// ロックオン
	void SetConfiguration(
		Math::Vector2	aPos,
		Math::Vector2	aMoveVal);	// 座標と移動量をシーンから設定する関数
	float GetAngleDeg(Math::Vector2 src,
		Math::Vector2 dest);	// 距離を求める関数
	void SetOwner(Scene* apOwner);
	void HitBullet();			//弾に当たった時の処理

private:	// 外部からアクセス不可
	KdTexture* m_pTexture;		// 画像(テクスチャ)
	Math::Vector2	m_pos;		// 座標
	Math::Vector2	m_moveVal;	// 移動量
	Math::Matrix	m_mat;		// 合成行列
	Math::Matrix	m_transMat;	// 移動行列
	Math::Matrix	m_scaleMat;	// 拡大行列
	float			m_scaleX;	// 拡大率(画像反転用)
	int				m_count;	// 敵の状態
	bool			m_bAlive;	// 生存フラグ
	const float		m_radius;	// 半径
	float			deg;		// 角度

	bool				HomingFlg;	// ホーミングするかしないか 

	void HitCheckMap();		//マップとの当たり判定

	Scene* m_pOwner;	// オーナー取得用

};

