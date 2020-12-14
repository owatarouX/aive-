#pragma once
#include "CPlayer.h"
#include "CEnemy.h"
#include "CMap.h"

//シーンの種類
enum eSceneType
{
	eSceneTitle,		//0	Title
	eSceneGame,			//1	ゲーム
	eSceneExplanation	//2	説明
};

//マウスの構造体
struct t_Mouse
{
	POINT cur;			//マウスカーソル座標
	POINT clickPoint;	//クリックした座標
	bool clickFlg;		//clickフラグ
};

//敵の要素数
constexpr int ENEMY_MAX = 3;

class Scene
{
private:

	//プレイヤー
	CPlayer m_player;
	
	//マップ
	CMap m_map;

	//敵
	CEnemy m_enemy[ENEMY_MAX];
	
	//テクスチャ関連
	KdTexture m_playerTex;
	KdTexture m_bulletTex;
	KdTexture m_slashTex;
	KdTexture m_bombTex;
	KdTexture m_mapTex;
	KdTexture m_enemyTex;
	KdTexture titleTex;
	KdTexture ExpTex;


	eSceneType sceneType;	//0:タイトル　1:ゲーム本編

	//マウス構造体
	t_Mouse mouse;

	//キー制御
	int keyFlg;		//0:押してない1:押している
	int clickFlg;	//マウスクリックフラグ

	
	Math::Matrix Titlemat;

	Math::Matrix Wmat;
	Math::Matrix Smat;

	Math::Color color;

	//タイトルシーン
	Math::Vector2 gameStartPos;			//タイトル:ゲームスタート座標
	bool gameStartFlg;					//タイトル:ゲームスタートフラグ
	Math::Vector2 ExplanationStartPos;	//タイトル:説明
	bool ExplanationStartFlg;			//タイトル:説明

	//説明シーン
	Math::Vector2 TitleStartPos;		//説明:タイトル
	bool TitleStartFlg;					//説明:タイトル

public:

	// 初期設定
	void Init();
	// 解放
	void Release();

	// 更新処理
	void Update();
	// 描画処理
	void Draw2D();

	// GUI処理
	void ImGuiUpdate();

	//敵クラス取得
	CEnemy* GetEnemy();

	//マップクラス取得
	CMap* GetMap();
	
	//プレイヤークラス取得
	CPlayer GetPlayer();
	
	//タイトル更新
	void TitleUpdate();

	//タイトル描画
	void TitleDraw();

	//ゲーム　更新
	void GameUpdate();

	//ゲーム描画
	void GameDraw();

	//説明　更新
	void ExplanationUpdate();

	//説明描画
	void ExplanationDraw();

	//マウス座標取得
	void GetMousePos();

private:
	DirectX::SpriteBatch* spriteBatch;
	DirectX::SpriteFont* spriteFont;

	Scene() {}
public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}
};

#define SCENE Scene::GetInstance()

// 点とOBBとの最近接点を求める
inline void PointToBox(const Math::Vector2& point, const Math::Vector2& obbCenterPos, const Math::Vector2& obbHalfSize, const Math::Matrix& obbMat, Math::Vector2& outNearestPos)
{
	// ※参考:[書籍]「ゲームプログラミングのためのリアルタイム衝突判定」
	Math::Vector2 d = point - obbCenterPos;
	// ボックスの中心における結果から開始、そのから段階的に進める。
	outNearestPos = obbCenterPos;
	// 各OBBの軸に対して
	for (int i = 0; i < 2; i++)
	{
		// dをその軸に投影して
		// ボックスの中心からdの軸に沿った距離を得る
		float dist = d.Dot((Math::Vector2&)obbMat.m[i]);
		// ボックスの範囲よりも距離が大きい場合、ボックスまでクランプ
		if (dist > (&obbHalfSize.x)[i]) dist = (&obbHalfSize.x)[i];
		if (dist < -(&obbHalfSize.x)[i]) dist = -(&obbHalfSize.x)[i];
		// ワールド座標を得るためにその距離だけ軸に沿って進める
		outNearestPos += dist * (Math::Vector2&)obbMat.m[i];
	}

}


//========================
// オブジェクト基本クラス
//========================
class ObjectBase
{
public:
	virtual ~ObjectBase() {}

	virtual void Update() {}
	virtual void Draw() {}

	virtual bool HitTest_Circle(const Math::Vector2& pos, float radius, Math::Vector2* hitPos) { return false; }

	virtual void DebugDraw() {}

	// 座標
	Math::Vector2	m_pos;
	float			m_z = 0;
};

//========================
// キャラ
//========================
class Chara : public ObjectBase
{
public:

	void Init(KdTexture* tex, float x, float y)
	{
		m_pTex = tex;
		m_pos.x = x;
		m_pos.y = y;
	}

	// 更新処理
	virtual void Update() override
	{
		// キー移動
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			m_pos.x -= 5.0f;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			m_pos.x += 5.0f;

		}

	}

	// 描画処理
	virtual void Draw() override
	{
		Math::Matrix m;
		// キャラの行列
		m = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, 0);

		// 行列セット
		SHADER.m_spriteShader.SetMatrix(m);
		// 描画
		SHADER.m_spriteShader.DrawTex(m_pTex, 0, 0, 48, 48, &Math::Rectangle(0, 0, 48, 48), &Math::Color(1,1,1,1), Math::Vector2(0.5f, 0.5f));
	}

private:
	KdTexture*	m_pTex = nullptr;
};
