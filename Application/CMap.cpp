#include"CMap.h"
#include"Scene.h"
#include"Utility.h"

CMap::CMap()
	:m_pTexture(nullptr)	//画像(テクスチャ)
	, m_ChipData()			//マップデータ
	, m_chipX()				//座標X
	, m_chipY()				//座標Y
	, m_mat()				//行列
	, m_scroll(0,0)			//スクロール量
{
}

CMap::~CMap()
{
}

//初期化
void CMap::Init()
{
	
	mapData = OutSide;

	//マップ情報ロード
	LoadMapFile();

	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			m_chipX[h][w] = (64.0f * w) - 640;
			m_chipY[h][w] = (-64.0f * h) + 360;
		}
	}
}

//更新
void CMap::Update()
{
	/*デバッグキー*/
	//仮スクロール処理
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_scroll.x -= 10.0f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_scroll.x += 10.0f;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_scroll.y += 10.0f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_scroll.y -= 10.0f;
	
	//マップ更新
	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			//スクロール処理
			Scroll();

			//画面端のリミット定義
			ScrollRemit();

		}
	}
	
	//行列作成
	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			m_mat[h][w] = DirectX::XMMatrixTranslation(
				m_chipX[h][w] - m_scroll.x, m_chipY[h][w] - m_scroll.y, 0.0f);
		}
	}
}

//描画
void CMap::Draw()
{
	Math::Rectangle chipRect;

	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			if (m_ChipData[h][w] == 0)		//壁
			{
				chipRect = { 0,0,64,64 };
			}
			else if (m_ChipData[h][w] == 1)
			{
				chipRect = { 64,0,64,64 };
			}
			else if (m_ChipData[h][w] == 2)
			{
				chipRect = { 128,0,64,64 };
			}
			else if (m_ChipData[h][w] == 3)
			{
				chipRect = { 192,0,64,64 };
			}
			else if (m_ChipData[h][w] == 4)
			{
				chipRect = { 256,0,64,64 };
			}
			else if (m_ChipData[h][w] == 5)
			{
				chipRect = { 320,0,64,64 };
			}
			else if (m_ChipData[h][w] == 7)
			{
				chipRect = { 384,0,64,64 };
			}
			else if (m_ChipData[h][w] == 6)
			{
				chipRect = {448 ,0,64,64 };
			}
			else if (m_ChipData[h][w] == 9)
			{
				chipRect = {512 ,0,64,64 };
			}
			else if (m_ChipData[h][w] == 8)
			{
				chipRect = { 576,0,64,64 };
			}
			SHADER.m_spriteShader.SetMatrix(m_mat[h][w]);
			SHADER.m_spriteShader.DrawTex(m_pTexture, chipRect, 1.0f);
		}
	}
}

//テクスチャ設定
void CMap::SetTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr) return;

	m_pTexture = apTexture;
}

//マップX座標取得
float(*CMap::GetPosX())[MAP_CHIP_W]
{
	return m_chipX;
}

//マップY座標取得
float(*CMap::GetPosY())[MAP_CHIP_W]
{
	return m_chipY;
}

//マップデータ取得
int(*CMap::GetChipData())[MAP_CHIP_W]
{
	return m_ChipData;
}

//スクロール量取得
Math::Vector2 CMap::GetscrollPos()
{
	return m_scroll;
}

//オーナー設定
void CMap::SetOwner(Scene* apOwner)
{
	if (apOwner == nullptr) return;

	m_pOwner = apOwner;
}

void CMap::SetMapData()
{
	if (mapData == OutSide)
	{
		mapData = OneFloor;
	}
	else if (mapData == OneFloor)
	{
		mapData = TwoFloor;
	}


}

//スクロール処理
void CMap::Scroll()
{
	//プレイヤー座標取得
	CPlayer player = m_pOwner->GetPlayer();
	Math::Vector2 playerPos = player.GetPos();

	//スクロール座標
	m_scroll = playerPos;
}

//画面端リミット定義
void CMap::ScrollRemit()
{
	static const float SCR_X_MIN = m_chipX[0][0] + 640;
	static const float SCR_X_MAX = m_chipX[0][MAP_CHIP_W - 1] - 640;

	static const float SCR_Y_MIN = m_chipY[MAP_CHIP_H - 1][0] + 360;
	static const float SCR_Y_MAX = m_chipY[0][0] - 360;

	//左端判定
	if (m_scroll.x < SCR_X_MIN)
		m_scroll.x = SCR_X_MIN; //端に到達時固定
	//右端判定
	else if (m_scroll.x > SCR_X_MAX)
		m_scroll.x = SCR_X_MAX; //端に到達時固定
	//下端判定
	if (m_scroll.y < SCR_Y_MIN)
		m_scroll.y = SCR_Y_MIN; //端に到達時固定
	//上端判定
	else if (m_scroll.y > SCR_Y_MAX)
		m_scroll.y = SCR_Y_MAX; //端に到達時固定
}


//マップ読み込み
void CMap::LoadMapFile()
{
	FILE* fp; // ファイルの情報を格納する構造体

	

	// ファイル読み込み
	if (mapData == OutSide)	//マップ１
	{
		if ((fp = fopen("Data/Map/map1-40.csv", "r")) != nullptr)
		{
			int h;
			int w;

			for (h = 0; h < 40; h++)
			{
				for (w = 0; w < 40; w++)
				{
					fscanf(fp, "%d,", &m_ChipData[h][w]);
				}
			}
			for (h = 0; h < 40; h++)
			{
				for (w = 0; w < 40; w++)
				{
					printf("%d ", &m_ChipData[h][w]);
				}
				printf("\n");
			}
			fclose(fp);
		}
	}
	else if (mapData == OneFloor)	//マップ２
	{
		//constexpr int MAP_CHIP_H = 26;					//縦サイズ
		//constexpr int MAP_CHIP_W = 28;					//横サイズ

		if ((fp = fopen("Data/Map/map40.txt", "r")) != nullptr)
		{
			char ch; // 文字入れ物
			int h = 0;
			int w = 0;

			while ((ch = fgetc(fp)) != EOF)
			{

				if (ch == '\n') // 改行の場合
				{
					h++;
					w = 0;
				}
				else //その他
				{
					//1文字を数字として格納 　↓文字から数字に変換
					m_ChipData[h][w] = ch - 0x30;
					w++;
				}
			}
			fclose(fp);
		}
	}
	


	// コンソール出力データを確認
	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			printf("%d", m_ChipData[h][w]);
		}
		printf("\n");
	}
}


//コンソール作成
void CMap::CreateConsole()
{
	// コンソールを割り当てる
	AllocConsole();

	//標準入出力を割り当てる
	freopen_s(&fp_c, "CONOUT$", "w", stdout);
	//	freopen_s(&fp_c, "CONIN$", "r", stdin);
}

// コンソール破壊
void CMap::DestroyConsole()
{
	fclose(fp_c);

	// コンソール破壊
	FreeConsole();
}
