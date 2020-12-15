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
	, m_HitFlg(false)
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

	//敵と当たったか判定
	m_HitFlg = false;

	//初期方向
	m_direction = Down;

	//初期装備
	m_LClick = eShuriken;
	m_RClick = eSword;

	//弾の初期化
	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].Init();
	}

	//刀初期化
	m_swordList.Init();

	//爆弾の初期化
	m_bombList.Init();

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
		m_direction = Up;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_moveVal.y -= MOVE_AMOUNT;
		m_direction = Down;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_moveVal.x -= MOVE_AMOUNT;
		m_direction = Left;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_moveVal.x += MOVE_AMOUNT;
		m_direction = Right;
	}


	//左クリック攻撃
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		switch (m_LClick)
		{
		case eSword:	//刀
			SetSword();
			break;
		case eShuriken:	//手裏剣
			SetShuriken();
			break;
		case eBomb:		//爆弾
			SetBomb();
			break;
		default:
			break;
		}

	}

	//右クリック攻撃
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		switch (m_RClick)
		{
		case eSword:
			SetSword();
			break;
		case eShuriken:
			SetShuriken();
			break;
		case eBomb:
			SetBomb();
			break;
		default:
			break;
		}

	}

	//仮発射
	SetBomb();
	//SetSword();

	//仮武器変更
	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_RClick = eBomb;
	}

	//仮マップデータ切り替え
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
		m_bulletList[i].SetScrollPos(ScrollPos);
		m_bulletList[i].Updata();
	}

	//刀攻撃更新
	m_swordList.SetScrollPos(ScrollPos);
	m_swordList.Updata(m_pos);

	//爆弾の更新
	m_bombList.SetScrollPos(ScrollPos);
	m_bombList.Updata();

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

	//斬撃の描画
	m_swordList.Draw();

	//爆弾の描画
	m_bombList.Draw();

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

//テクスチャ設定:斬撃
void CPlayer::SetSlashTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_swordList.SetTexture(apTexture);
}

//テクスチャ設定:爆弾
void CPlayer::SetBombTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_bombList.SetTexture(apTexture);
}

//テクスチャ設定：爆発
void CPlayer::SetBlastTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_bombList.SetBlastTexture(apTexture);
}

//オーナー設定
void CPlayer::SetOwner(Scene* apOwner)
{
	if (apOwner == nullptr) return;

	m_pOwner = apOwner;
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

Math::Vector2 CPlayer::GetBlast()
{
	return m_bombList.GetBlastPos();
}

Math::Vector2 CPlayer::GetBomb()
{
	return m_bombList.GetPos();
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
			if (chipData[h][w] >= 6)
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
	for (int e = 0; e < ENEMY_MAX; e++)
	{
		CEnemy* enemy = m_pOwner->GetEnemy()+e;		//敵クラス取得
		Math::Vector2 enePos = enemy->GetPos();		//敵の座標取得

		//生きてる敵のみ
		if (enemy->IsAlive())
		{
			////////////////////////////////////////////////////////////////
			//		プレイヤーのヒットチェック								
			////////////////////////////////////////////////////////////////
			if (!m_HitFlg)
			{
				int player_hit = Utility::iHitCheck(m_pos, m_moveVal, enePos.x, enePos.y, Infor::RADIUS_32, Infor::RADIUS_32);

				//敵の現在座標の四辺
				const float ENEMY_LEFT = enePos.x - Infor::RADIUS_32;	//左辺
				const float ENEMY_RIGHT = enePos.x + Infor::RADIUS_32;	//右辺
				const float ENEMY_TOP = enePos.y + Infor::RADIUS_32;		//上辺
				const float ENEMY_BOTTOM = enePos.y - Infor::RADIUS_32;	//下辺

				const float KnockBack = 40;		//ノックバック量
				const float ALPHA = 0.5;		//ヒット時の透過値
				//当たり判定分岐処理
				//1:上	2:下 3:左 4:右
				switch (player_hit)
				{
				case 1:
					m_pos.y = ENEMY_TOP + Infor::RADIUS_32 + KnockBack;
					m_moveVal.y = 0;
					m_alpha = ALPHA;	//半透明化
					m_hp -= 1;			//体力減少
					m_HitFlg = true;
					break;
				case 2:
					m_pos.y = ENEMY_BOTTOM - Infor::RADIUS_32 - KnockBack;
					m_moveVal.y = 0;
					m_alpha = ALPHA;
					m_hp -= 1;
					m_HitFlg = true;
					break;
				case 3:
					m_pos.x = ENEMY_LEFT - Infor::RADIUS_32 - KnockBack;
					m_moveVal.x = 0;
					m_alpha = ALPHA;
					m_hp -= 1;
					m_HitFlg = true;
					break;
				case 4:
					m_pos.x = ENEMY_RIGHT + Infor::RADIUS_32 + KnockBack;
					m_moveVal.x = 0;
					m_alpha = ALPHA;
					m_hp -= 1;
					m_HitFlg = true;
					break;
				default:
					break;
				}
			}
			////////////////////////////////////////////////////////////////
			//		弾のヒットチェック								
			////////////////////////////////////////////////////////////////
			for (int i = 0; i < BULLET_MAX; i++)
			{
				//生きてる弾のみ
				if (m_bulletList[i].IsAlive())
				{
					bool bullet_hit = true;
					bullet_hit = Utility::bHitCheck(m_bulletList[i].GetPos(), m_bulletList[i].GetMove(), enePos, Infor::RADIUS_8, Infor::RADIUS_32);

					//ヒット時
					if (!bullet_hit)
					{
						enemy->HitBullet();					//敵のフラグ下げ
						m_bulletList[i].SetAlive(false);	//弾のフラグ下げ
						break;
					}
				}
			}
			////////////////////////////////////////////////////////////////
			//		斬撃のヒットチェック								
			////////////////////////////////////////////////////////////////

			bool slash_hit = true;
			slash_hit = Utility::bHitCheck(m_swordList.GetPos(), m_swordList.GetMove(), enePos, Infor::RADIUS_32, Infor::RADIUS_32);

			//ヒット時
			if (!slash_hit)
			{
				enemy->HitBullet();				//敵のフラグ下げ
			}
			////////////////////////////////////////////////////////////////
			//		爆発のヒットチェック								
			////////////////////////////////////////////////////////////////

			//爆発している時
			if (m_bombList.GetBlastAlive())
			{
				bool blast_hit = true;
				blast_hit = Utility::bHitCheck(m_bombList.GetBlastPos(), { 0,0 }, enePos, Infor::RADIUS_32, Infor::RADIUS_32);

				//ヒット時
				if (!blast_hit)
				{
					enemy->HitBullet();				//敵のフラグ下げ
				}
			}
		}
	}
}

//無敵時間
void CPlayer::InviTime()
{
	if (m_HitFlg)	//半透明状態なら
	{
		const int CNT_MAX = 120;	//無敵時間
		if (m_hpCount >= CNT_MAX)
		{
			m_alpha = 1.0f;
			m_hpCount = 0;
			m_HitFlg = false;
		}
		m_hpCount++;
	}
}


//攻撃：手裏剣
void CPlayer::SetShuriken()
{
	CMap* map = m_pOwner->GetMap();
	Math::Vector2 ScrollPos = map->GetscrollPos();

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!m_bulletList[i].IsAlive())
		{
			m_bulletList[i].Shot(m_pos /*- ScrollPos*/, m_direction);
			break;
		}
	}
}

//攻撃：刀
void CPlayer::SetSword()
{
	CMap* map = m_pOwner->GetMap();
	Math::Vector2 ScrollPos = map->GetscrollPos();

	if (!m_swordList.bGetSlash())
	{
		m_swordList.Slash(m_pos - ScrollPos, m_direction);
	}
}

//攻撃：爆弾
void CPlayer::SetBomb()
{
	CMap* map = m_pOwner->GetMap();
	Math::Vector2 ScrollPos = map->GetscrollPos();

	if (!m_bombList.IsAlive())
	{
		m_bombList.InstBomb(m_pos/* - ScrollPos*/);
	}
}

