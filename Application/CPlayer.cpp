#include "CPlayer.h"
#include "Scene.h"
#include"Utility.h"

//�R���X�g���N�^
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

//������
void CPlayer::Init()
{
	//���W
	m_pos.x = -300.0f;
	m_pos.y = -1080.0f;
	
	//�ړ���
	m_moveVal.x = 0.0f;
	m_moveVal.y = 0.0f;
	
	//�����t���O
	m_bAlive = true;
	
	//�̗�
	m_hp = 5;
	m_hpCount = 0;

	//�����x
	m_alpha = 1.0f;

	//�G�Ɠ�������������
	m_HitFlg = false;

	//��������
	m_direction = Down;

	//��������
	m_LClick = eShuriken;
	m_RClick = eSword;

	//�e�̏�����
	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].Init();
	}

	//��������
	m_swordList.Init();

	//���e�̏�����
	m_bombList.Init();

}

//�X�V
void CPlayer::Updata()
{
	//�}�b�v�N���X�擾
	CMap* map = m_pOwner->GetMap();


	//�������̂ݏ���
	if (!m_bAlive) return;
	
	m_moveVal = { 0,0 };

	//�ړ���
	const float MOVE_AMOUNT = 5;

	//�X�N���[���ʎ擾
	Math::Vector2 ScrollPos = map->GetscrollPos();

	/* �L�[���� */

	//�ړ�
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


	//���N���b�N�U��
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		switch (m_LClick)
		{
		case eSword:	//��
			SetSword();
			break;
		case eShuriken:	//�藠��
			SetShuriken();
			break;
		case eBomb:		//���e
			SetBomb();
			break;
		default:
			break;
		}

	}

	//�E�N���b�N�U��
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

	//������
	SetBomb();
	//SetSword();

	//������ύX
	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_RClick = eBomb;
	}

	//���}�b�v�f�[�^�؂�ւ�
	if (m_pos.x >= 512 && m_pos.x <= 540 &&
		m_pos.y >= 104 && m_pos.y <= 232)
	{
		map->SetMapData();
		map->LoadMapFile();
		m_pos.x = -515;
		m_pos.y = -2000;
	}

	//�G�Ƃ̓����蔻��
	HitCheckEnemy();
	
	//�}�b�v�Ƃ̓����蔻��
	HitCheckMap();

	//���G����
	InviTime();

	//HP0�Ńt���O����
	if (m_hp <= 0) m_bAlive = !m_bAlive;

	
	//���W�m��
	m_pos.x += m_moveVal.x;
	m_pos.y += m_moveVal.y;
	
	//�s��쐬
	m_mat = DirectX::XMMatrixTranslation(m_pos.x - ScrollPos.x, m_pos.y - ScrollPos.y, 0.0f);


	
	//�e�̍X�V
	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].SetScrollPos(ScrollPos);
		m_bulletList[i].Updata();
	}

	//���U���X�V
	m_swordList.SetScrollPos(ScrollPos);
	m_swordList.Updata(m_pos);

	//���e�̍X�V
	m_bombList.SetScrollPos(ScrollPos);
	m_bombList.Updata();

}

//�`��
void CPlayer::Draw()
{
	if (!m_bAlive) return;

	//�e�̕`��
	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].Draw();
	}

	//�a���̕`��
	m_swordList.Draw();

	//���e�̕`��
	m_bombList.Draw();

	//�v���C���[�`��
	SHADER.m_spriteShader.SetMatrix(m_mat);
	Math::Rectangle scrRect = { 0,0,64,64 }; // �e�N�X�`�����W
	Math::Color color = { 1,1,1, m_alpha }; // �F�iRGBA�̏��ԂŁ@0.0�`1.0�j
	SHADER.m_spriteShader.DrawTex(m_pTexture, 0, 0, 64, 64, &scrRect, &color, Math::Vector2(0.5f, 0.5f));

}

//�t���O��Ԏ擾
const bool CPlayer::IsAlive()
{
	return m_bAlive;
}

//�e�N�X�`���ݒ�:���@
void CPlayer::SetTexture(KdTexture* apTexture)
{
	//�|�C���^�̃A�h���X�����퉻����ׂ� (null�Ȃ�Z�b�g���Ȃ�)
	if (apTexture == nullptr)return;

	m_pTexture = apTexture;

}

//�e�N�X�`���ݒ�:�藠��
void CPlayer::SetBulletTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		m_bulletList[i].SetTexture(apTexture);
	}
}

//�e�N�X�`���ݒ�:�a��
void CPlayer::SetSlashTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_swordList.SetTexture(apTexture);
}

//�e�N�X�`���ݒ�:���e
void CPlayer::SetBombTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_bombList.SetTexture(apTexture);
}

//�e�N�X�`���ݒ�F����
void CPlayer::SetBlastTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr)return;

	m_bombList.SetBlastTexture(apTexture);
}

//�I�[�i�[�ݒ�
void CPlayer::SetOwner(Scene* apOwner)
{
	if (apOwner == nullptr) return;

	m_pOwner = apOwner;
}


//���@���W�擾
const Math::Vector2 CPlayer::GetPos()
{
	return m_pos;
}

//���@HP�擾
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

//�}�b�v�Ƃ̓����蔻��
void CPlayer::HitCheckMap()
{
	CMap* map = m_pOwner->GetMap();

	float(*chipX)[MAP_CHIP_W] = map->GetPosX();
	float(*chipY)[MAP_CHIP_W] = map->GetPosY();
	int(*chipData)[MAP_CHIP_W] = map->GetChipData();

	//�}�b�v�f�[�^���g����
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
				//�}�b�v�̌��ݍ��W�̎l��
				const float MAP_LEFT = chipX[h][w] - Infor::RADIUS_32;		//����
				const float MAP_RIGHT = chipX[h][w] + Infor::RADIUS_32;		//�E��
				const float MAP_TOP = chipY[h][w] + Infor::RADIUS_32;		//���
				const float MAP_BOTTOM = chipY[h][w] - Infor::RADIUS_32;	//����

				////////////////////////////////////////////////////////////////
				//		�v���C���[�̃q�b�g�`�F�b�N								
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
				//		�e�̃q�b�g�`�F�b�N								
				////////////////////////////////////////////////////////////////
				for (int i = 0; i < BULLET_MAX; i++)
				{
					bool hit = Utility::bHitCheck(m_bulletList[i].GetPos(), m_bulletList[i].GetMove(), { chipX[h][w],chipY[h][w] }, Infor::RADIUS_8, Infor::RADIUS_32);
					
					//�q�b�g��
					if (!hit)
					{
						m_bulletList[i].SetAlive(false);	//�e�̃t���O����
					}
				}
			}
		}
	}
}

//�G�Ƃ̓����蔻��
void CPlayer::HitCheckEnemy()
{
	for (int e = 0; e < ENEMY_MAX; e++)
	{
		CEnemy* enemy = m_pOwner->GetEnemy()+e;		//�G�N���X�擾
		Math::Vector2 enePos = enemy->GetPos();		//�G�̍��W�擾

		//�����Ă�G�̂�
		if (enemy->IsAlive())
		{
			////////////////////////////////////////////////////////////////
			//		�v���C���[�̃q�b�g�`�F�b�N								
			////////////////////////////////////////////////////////////////
			if (!m_HitFlg)
			{
				int player_hit = Utility::iHitCheck(m_pos, m_moveVal, enePos.x, enePos.y, Infor::RADIUS_32, Infor::RADIUS_32);

				//�G�̌��ݍ��W�̎l��
				const float ENEMY_LEFT = enePos.x - Infor::RADIUS_32;	//����
				const float ENEMY_RIGHT = enePos.x + Infor::RADIUS_32;	//�E��
				const float ENEMY_TOP = enePos.y + Infor::RADIUS_32;		//���
				const float ENEMY_BOTTOM = enePos.y - Infor::RADIUS_32;	//����

				const float KnockBack = 40;		//�m�b�N�o�b�N��
				const float ALPHA = 0.5;		//�q�b�g���̓��ߒl
				//�����蔻�蕪�򏈗�
				//1:��	2:�� 3:�� 4:�E
				switch (player_hit)
				{
				case 1:
					m_pos.y = ENEMY_TOP + Infor::RADIUS_32 + KnockBack;
					m_moveVal.y = 0;
					m_alpha = ALPHA;	//��������
					m_hp -= 1;			//�̗͌���
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
			//		�e�̃q�b�g�`�F�b�N								
			////////////////////////////////////////////////////////////////
			for (int i = 0; i < BULLET_MAX; i++)
			{
				//�����Ă�e�̂�
				if (m_bulletList[i].IsAlive())
				{
					bool bullet_hit = true;
					bullet_hit = Utility::bHitCheck(m_bulletList[i].GetPos(), m_bulletList[i].GetMove(), enePos, Infor::RADIUS_8, Infor::RADIUS_32);

					//�q�b�g��
					if (!bullet_hit)
					{
						enemy->HitBullet();					//�G�̃t���O����
						m_bulletList[i].SetAlive(false);	//�e�̃t���O����
						break;
					}
				}
			}
			////////////////////////////////////////////////////////////////
			//		�a���̃q�b�g�`�F�b�N								
			////////////////////////////////////////////////////////////////

			bool slash_hit = true;
			slash_hit = Utility::bHitCheck(m_swordList.GetPos(), m_swordList.GetMove(), enePos, Infor::RADIUS_32, Infor::RADIUS_32);

			//�q�b�g��
			if (!slash_hit)
			{
				enemy->HitBullet();				//�G�̃t���O����
			}
			////////////////////////////////////////////////////////////////
			//		�����̃q�b�g�`�F�b�N								
			////////////////////////////////////////////////////////////////

			//�������Ă��鎞
			if (m_bombList.GetBlastAlive())
			{
				bool blast_hit = true;
				blast_hit = Utility::bHitCheck(m_bombList.GetBlastPos(), { 0,0 }, enePos, Infor::RADIUS_32, Infor::RADIUS_32);

				//�q�b�g��
				if (!blast_hit)
				{
					enemy->HitBullet();				//�G�̃t���O����
				}
			}
		}
	}
}

//���G����
void CPlayer::InviTime()
{
	if (m_HitFlg)	//��������ԂȂ�
	{
		const int CNT_MAX = 120;	//���G����
		if (m_hpCount >= CNT_MAX)
		{
			m_alpha = 1.0f;
			m_hpCount = 0;
			m_HitFlg = false;
		}
		m_hpCount++;
	}
}


//�U���F�藠��
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

//�U���F��
void CPlayer::SetSword()
{
	CMap* map = m_pOwner->GetMap();
	Math::Vector2 ScrollPos = map->GetscrollPos();

	if (!m_swordList.bGetSlash())
	{
		m_swordList.Slash(m_pos - ScrollPos, m_direction);
	}
}

//�U���F���e
void CPlayer::SetBomb()
{
	CMap* map = m_pOwner->GetMap();
	Math::Vector2 ScrollPos = map->GetscrollPos();

	if (!m_bombList.IsAlive())
	{
		m_bombList.InstBomb(m_pos/* - ScrollPos*/);
	}
}

