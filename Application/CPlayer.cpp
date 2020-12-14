#include "CPlayer.h"
#include "Scene.h"
#include"Utility.h"

//コンストラクタ
CPlayer::CPlayer()
	:m_pTexture(nullptr)
	,m_pos(0.0f,0.0f)
	,m_moveVal(0.0f,0.0f)
	,m_mat()
	,m_bAlive(true)
	,m_hp(0)
	,m_hpCount()
	, m_alpha(1.0f)
{
}

CPlayer::~CPlayer()
{
}

//初期化
void CPlayer::Init()
{
	//座標
	m_pos.x = -300.0f;
	m_pos.y = -1080.0f;
	
	//移動量
	m_moveVal.x = 0.0f;
	m_moveVal.y = 0.0f;
	
	//生存フラグ
	m_bAlive = true;
	
	//体力
	m_hp = 5;
	m_hpCount = 0;

	//透明度
	m_alpha = 1.0f;

	direction = Down;

	//弾の初期化
	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].Init();
		m_bulletList[i].SetOwner(m_pOwner);
	}

}

//更新
void CPlayer::Updata()
{
	//マップクラス取得
	CMap* map = m_pOwner->GetMap();


	//生存時のみ処理
	if (!m_bAlive) return;
	
	m_moveVal = { 0,0 };

	//移動量
	const float MOVE_AMOUNT = 5;

	//スクロール量取得
	Math::Vector2 ScrollPos = map->GetscrollPos();

	/* キー入力 */

	//移動
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_moveVal.y += MOVE_AMOUNT;
		direction = Up;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_moveVal.y -= MOVE_AMOUNT;
		direction = Down;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_moveVal.x -= MOVE_AMOUNT;
		direction = Left;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_moveVal.x += MOVE_AMOUNT;
		direction = Right;
	}

	//手裏剣発射
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (!m_bulletList[i].IsAlive())
			{
				m_bulletList[i].Shot(m_pos - ScrollPos, direction);
				break;
			}
		}
	}

	//マップデータ切り替え
	/*if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		map->SetMapData();
		map->LoadMapFile();
	}*/
	if (m_pos.x >= 512 && m_pos.x <= 540 &&
		m_pos.y >= 104 && m_pos.y <= 232)
	{
		map->SetMapData();
		map->LoadMapFile();
		m_pos.x = -515;
		m_pos.y = -2000;
	}
	

	//敵との当たり判定
	HitCheckEnemy();
	
	//マップとの当たり判定
	HitCheckMap();

	//無敵時間
	InviTime();

	//HP0でフラグ下げ
	if (m_hp <= 0) m_bAlive = !m_bAlive;

	
	//座標確定
	m_pos.x += m_moveVal.x;
	m_pos.y += m_moveVal.y;
	
	//行列作成
	m_mat = DirectX::XMMatrixTranslation(m_pos.x - ScrollPos.x, m_pos.y - ScrollPos.y, 0.0f);


	//弾の更新
	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].Updata();
	}

}

//描画
void CPlayer::Draw()
{
	if (!m_bAlive) return;

	//弾の描画
	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].Draw();
	}

	//プレイヤー描画
	SHADER.m_spriteShader.SetMatrix(m_mat);
	Math::Rectangle scrRect = { 0,0,64,64 }; // テクスチャ座標
	Math::Color color = { 1,1,1, m_alpha }; // 色（RGBAの順番で　0.0～1.0）
	SHADER.m_spriteShader.DrawTex(m_pTexture, 0, 0, 64, 64, &scrRect, &color, Math::Vector2(0.5f, 0.5f));

}

//フラグ状態取得
const bool CPlayer::IsAlive()
{
	return m_bAlive;
}

//テクスチャ設定:自機
void CPlayer::SetTexture(KdTexture* apTexture)
{
	//ポインタのアドレスが正常化しらべる (nullならセットしない)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;

}

//テクスチャ設定:手裏剣
void CPlayer::SetBulletTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].SetTexture(apTexture);
	}
}

//オーナー設定
void CPlayer::SetOwner(Scene* apOwner)
{
	if (apOwner == nullptr) return;

	m_pOwner = apOwner;
}

//スクロール方向取得
int CPlayer::SetScrollDirect()
{
	return direction;
}

//自機座標取得
const Math::Vector2 CPlayer::GetPos()
{
	return m_pos;
}

//自機HP取得
const int CPlayer::GetHp()
{
	return m_hp;
}

//マップとの当たり判定
void CPlayer::HitCheckMap()
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
			if (chipData[h][w] >= 9)
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
				//		プレイヤーのヒットチェック								
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
				////////////////////////////////////////////////////////////////
				//		弾のヒットチェック								
				////////////////////////////////////////////////////////////////
				for (int i = 0; i < BULLET_MAX; i++)
				{
					bool hit = Utility::bHitCheck(m_bulletList[i].GetPos(), m_bulletList[i].GetMove(), { chipX[h][w],chipY[h][w] }, Infor::RADIUS_8, Infor::RADIUS_32);
					
					//ヒット時
					if (!hit)
					{
						m_bulletList[i].SetAlive(false);	//弾のフラグ下げ
					}
				}
			}
		}
	}
}

//敵との当たり判定
void CPlayer::HitCheckEnemy()
{
	if (m_alpha < 1) return;	//無敵時間があるとき
	
	for (int e = 0; e < ENEMY_MAX; e++)
	{
		CEnemy enemy = m_pOwner->GetEnemy()[e];		//敵クラス取得
		Math::Vector2 enePos = enemy.GetPos();		//敵の座標取得

		////////////////////////////////////////////////////////////////
		//		プレイヤーのヒットチェック								
		////////////////////////////////////////////////////////////////
		int hit = Utility::iHitCheck(m_pos, m_moveVal, enePos.x, enePos.y, Infor::RADIUS_32, Infor::RADIUS_32);

		//敵の現在座標の四辺
		const float ENEMY_LEFT = enePos.x - Infor::RADIUS_32;	//左辺
		const float ENEMY_RIGHT = enePos.x + Infor::RADIUS_32;	//右辺
		const float ENEMY_TOP = enePos.y + Infor::RADIUS_32;		//上辺
		const float ENEMY_BOTTOM = enePos.y - Infor::RADIUS_32;	//下辺

		const float KnockBack = 40;		//ノックバック量
		const float ALPHA = 0.5;		//ヒット時の透過値
		//当たり判定分岐処理
		//1:上	2:下 3:左 4:右
		switch (hit)
		{
		case 1:
			m_pos.y = ENEMY_TOP + Infor::RADIUS_32 + KnockBack;
			m_moveVal.y = 0;
			m_alpha = ALPHA;	//半透明化
			//m_hp -= 1;			//体力減少
			break;
		case 2:
			m_pos.y = ENEMY_BOTTOM - Infor::RADIUS_32 - KnockBack;
			m_moveVal.y = 0;
			m_alpha = ALPHA;
			//m_hp -= 1;
			break;
		case 3:
			m_pos.x = ENEMY_LEFT - Infor::RADIUS_32 - KnockBack;
			m_moveVal.x = 0;
			m_alpha = ALPHA;
			//m_hp -= 1;
			break;
		case 4:
			m_pos.x = ENEMY_RIGHT + Infor::RADIUS_32 + KnockBack;
			m_moveVal.x = 0;
			m_alpha = ALPHA;
			//m_hp -= 1;
			break;
		default:
			break;
		}

		////////////////////////////////////////////////////////////////
		//		弾のヒットチェック								
		////////////////////////////////////////////////////////////////
		for (int i = 0;i < BULLET_MAX;i++)
		{
			bool hit = true;
			hit	= Utility::bHitCheck(m_bulletList[i].GetPos(), m_bulletList[i].GetMove(), enePos, Infor::RADIUS_8, Infor::RADIUS_32);
			
			//ヒット時
			if (!hit)
			{
				enemy.HitBullet();					//敵のフラグ下げ
				m_bulletList[i].SetAlive(false);	//弾のフラグ下げ
			}
		}
	}
}

//無敵時間
void CPlayer::InviTime()
{
	if (m_alpha < 1)	//半透明状態なら
	{
		const int CNT_MAX = 60;	//無敵時間
		if (m_hpCount >= CNT_MAX)
		{
			m_alpha = 1.0f;
			m_hpCount = 0;
		}
		m_hpCount++;
	}
}



