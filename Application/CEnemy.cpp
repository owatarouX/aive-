#include"CEnemy.h"
#include "Scene.h"
#include"Utility.h"

// コンストラクタ
CEnemy::CEnemy()
	: m_pTexture(nullptr)
	, m_pos(0.0f, 0.0f)
	, m_moveVal(0.0f, 0.0f)
	, m_mat()
	, m_transMat()
	, m_scaleMat()
	, m_scaleX(0.0f)
	, m_count(0)
	, m_bAlive(true)
	, m_radius(32.0f)
	, deg(0.0f)
	, HomingFlg(true)
{
}

// デストラクタ
CEnemy::~CEnemy()
{
}

// 初期化
void CEnemy::Init()
{
	m_scaleX = -1.0f;		// 拡大率(画像反転用)

	m_count = 0;			// 敵の状態をカウントの値で管理

	m_bAlive = true;		// フラグ

	deg = 0.0f;				// 角度
	HomingFlg = true;

}

// 更新
void CEnemy::Update()
{
	if (!m_bAlive) return;	// フラグ確認
	
	m_pos.x += m_moveVal.x;
	m_pos.y += m_moveVal.y;

	//Moveleftandright();
	
	if (HomingFlg)// ホーミングするか？
	{
		Homing();
		//HomingFlg = false;	// 一度だけしかホーミングしない
	//  ↑の1行を消せば常にホーミングする
	}

	//マップとの当たり判定
	HitCheckMap();

	CMap* map = m_pOwner->GetMap();
	//スクロール量取得
	Math::Vector2 scrollPos = map->GetscrollPos();

	//行列
	m_transMat = DirectX::XMMatrixTranslation(m_pos.x - scrollPos.x, m_pos.y - scrollPos.y, 0.0f);

	m_scaleMat = DirectX::XMMatrixScaling(m_scaleX, 1.0f, 0.0f);

	m_mat = m_scaleMat * m_transMat;	// 行列作成
}

// 描画
void CEnemy::Draw()
{
	if (!m_bAlive) return;	// フラグ確認

	SHADER.m_spriteShader.SetMatrix(m_mat);	// 行列背セット
	SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(0, 0, 64, 64), 1.0f);	// 描画
}

// 生存確認
const bool CEnemy::IsAlive()
{
	return m_bAlive;
}

// テクスチャ受け取り
void CEnemy::SetTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr) return;	// ポインタのアドレスが正常か調べる(nullならセットしない)

	m_pTexture = apTexture;
}

//座標取得
Math::Vector2 CEnemy::GetPos()
{
	return m_pos;
}

// 左右移動 
void CEnemy::Moveleftandright()
{
	m_count++;

	if (m_count < 20)
	{
		m_pos.x += m_moveVal.x;
	}
	else if (m_count < 30)
	{
		m_scaleX = -1;
	}
	else if (m_count < 40)
	{
		m_scaleX = 1;
	}
	else if (m_count < 60)
	{
		m_pos.x -= m_moveVal.x;
	}
	else if (m_count < 70)
	{
		m_scaleX = -1;
	}
	else
	{
		m_count = 0;
	}
}

// ロックオン
void CEnemy::Homing()
{
	CPlayer player = m_pOwner->GetPlayer();		// プレイヤークラスの取得
	Math::Vector2 playerPos = player.GetPos();	// プレイヤー座標をplayerPosに入力

	deg = GetAngleDeg(m_pos,playerPos);

	m_moveVal.x = cos(DirectX::XMConvertToRadians(deg)) * 1.8;	// cos
	m_moveVal.y = sin(DirectX::XMConvertToRadians(deg)) * 1.8;	// sin
}

// 座標と移動量をシーンから設定する関数
void CEnemy::SetConfiguration(Math::Vector2 aPos, Math::Vector2 aMoveVal)
{
	m_pos = aPos;
	m_moveVal = aMoveVal;

}

float CEnemy::GetAngleDeg(Math::Vector2 src,Math::Vector2 dest)
{
	float a;	// 底辺
	float b;	// 高さ
	float rad;	// ラジアン
	float deg;	// デグリー

	// 引く順番に注意！：目的地 - 元で求める必要がある
	a = dest.x - src.x;
	b = dest.y - src.y;

	// tanの逆関数で角度を求める  atan2(Y,X)引数の順番に注意
	rad = atan2f(b, a);

	//デグリーへ変換(計算式または関数利用)
	deg = DirectX::XMConvertToDegrees(rad);
	//deg = rad * 180.0f / 3.141592f;

	// 負の値の場合は正の値に補正(これで必ず0～369になる)
	if (deg < 0)
	{
		deg += 360;
	}

	//角度を返す
	return deg;
}

void CEnemy::SetOwner(Scene* apOwner)
{
	if (apOwner == nullptr) return;

	m_pOwner = apOwner;
}

//弾に当たった時の処理
void CEnemy::HitBullet()
{
	m_bAlive = false;
}

//マップとの当たり判定
void CEnemy::HitCheckMap()
{
	CMap* map = m_pOwner->GetMap();

	float(*chipX)[MAP_CHIP_W] = map->GetPosX();
	float(*chipY)[MAP_CHIP_W] = map->GetPosY();
	int(*chipData)[MAP_CHIP_W] = map->GetChipData();

	//マップデータを使って
	int hit = 0;
	for (int h = 0; h < MAP_CHIP_H; h++)
	{
		for (int w = 0; w < MAP_CHIP_W; w++)
		{
			if (chipData[h][w] >= 1)
			{
				continue;
			}
			else
			{
				//マップの現在座標の四辺
				const float MAP_LEFT = chipX[h][w] - Infor::RADIUS_32;		//左辺
				const float MAP_RIGHT = chipX[h][w] + Infor::RADIUS_32;		//右辺
				const float MAP_TOP = chipY[h][w] + Infor::RADIUS_32;		//上辺
				const float MAP_BOTTOM = chipY[h][w] - Infor::RADIUS_32;	//下辺

				////////////////////////////////////////////////////////////////
				//		敵のヒットチェック								
				////////////////////////////////////////////////////////////////
				hit = Utility::iHitCheck(m_pos, m_moveVal, chipX[h][w], chipY[h][w], Infor::RADIUS_32, Infor::RADIUS_32);

				switch (hit)
				{
				case 1:m_pos.y = MAP_TOP + Infor::RADIUS_32;
					m_moveVal.y = 0;
					break;
				case 2:m_pos.y = MAP_BOTTOM - Infor::RADIUS_32;
					m_moveVal.y = 0;
					break;
				case 3:m_pos.x = MAP_LEFT - Infor::RADIUS_32;
					m_moveVal.x = 0;
					break;
				case 4:m_pos.x = MAP_RIGHT + Infor::RADIUS_32;
					m_moveVal.x = 0;
				break; default:
					break;
				}
			}
		}
	}
}

////敵との当たり判定
//void CPlayer::HitCheckEnemy()
//{
//	for (int e = 0; e < ENEMY_MAX; e++)
//	{
//		////////////////////////////////////////////////////////////////
//		//		弾のヒットチェック								
//		////////////////////////////////////////////////////////////////
//		for (int i = 0; i < BULLET_MAX; i++)
//		{
//			bool hit = true;
//			hit = Utility::bHitCheck(m_bulletList[i].GetPos(), m_bulletList[i].GetMove(), m_pos, Infor::RADIUS_8, Infor::RADIUS_32);
//
//			//ヒット時
//			if (!hit)
//			{
//				m_bAlive = false;					//敵のフラグ下げ
//				m_bulletList[i].SetAlive(false);	//弾のフラグ下げ
//			}
//		}
//	}
//}