#pragma once

//マップシーン
enum eSceneData
{
	OutSide,	//城外
	OneFloor,	//1階層
	TwoFloor	//2階層
};

class Scene;

constexpr int MAP_CHIP_H = 40;					//縦サイズ
constexpr int MAP_CHIP_W = 40;					//横サイズ

class CMap
{
public:
	CMap();
	~CMap();

	void Init();
	void Update();
	void Draw();
	void SetTexture(KdTexture* apTexture);
	float (*GetPosX())[MAP_CHIP_W];		//X座標取得
	float (*GetPosY())[MAP_CHIP_W];		//Y座標取得
	int (*GetChipData())[MAP_CHIP_W];	//マップデータ取得
	Math::Vector2 GetscrollPos();		//スクロール量取得
	void SetOwner(Scene* apOwner);		//オーナー設定
	void SetMapData();					//マップデータの変更

	void LoadMapFile();		//マップファイル読み込み
	void CreateConsole();	// コンソール作成
	void DestroyConsole();	// コンソール破壊

private:

	eSceneData			mapData;

	KdTexture*			m_pTexture;								//画像(テクスチャ)
	int					m_ChipData[MAP_CHIP_H][MAP_CHIP_W];		//マップデータ
	float				m_chipX[MAP_CHIP_H][MAP_CHIP_W];		//座標
	float				m_chipY[MAP_CHIP_H][MAP_CHIP_W];		//座標
	Math::Matrix		m_mat[MAP_CHIP_H][MAP_CHIP_W];			//行列
	Math::Vector2		m_scroll;								//スクロール量
	Scene* m_pOwner;											//オーナー取得用

	void Scroll();			//スクロール処理
	void ScrollRemit();		//画面端リミット

	FILE* fp_c;			//コンソール用
	

};
