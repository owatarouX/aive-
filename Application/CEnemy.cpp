#include"CEnemy.h"
#include "Scene.h"
#include"Utility.h"

// �R���X�g���N�^
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
	, VisibilityFlg(false)
{
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
}

// ������
void CEnemy::Init()
{
	m_scaleX = -1.0f;		// �g�嗦(�摜���]�p)

	m_count = 0;			// �G�̏�Ԃ��J�E���g�̒l�ŊǗ�

	m_bAlive = true;		// �t���O

	deg = 0.0f;				// �p�x

	HomingFlg = true;		// �ǐՂ��邩

	VisibilityFlg=false;	// �~����ɓ�������
}

// �X�V
void CEnemy::Update()
{
	if (!m_bAlive) return;	// �t���O�m�F
	
	CPlayer player = m_pOwner->GetPlayer();		// �v���C���[�N���X�̎擾
	Math::Vector2 playerPos = player.GetPos();	// �v���C���[���W��playerPos�ɓ���

	if (GetDistance(m_pos, playerPos) < 200)	// GetDistance�ň�����̋����𑪂�
												// 200�Ȃ��Ȃ�if�����̃v���O���������s
	{
		VisibilityFlg = true;					// �͈͓�Flg�𗧂Ă�
	}
	if (VisibilityFlg)							// �͈͓��Ȃ�
	{
		m_pos.x += m_moveVal.x;	// X += moveX
		m_pos.y += m_moveVal.y;	// Y += moveY
		Homing();				// �ǐՊ֐��Ăяo��
	}

	//Moveleftandright();
	
	/*if (HomingFlg)// �z�[�~���O���邩�H
	{
		Homing();
		//HomingFlg = false;	// ��x���������z�[�~���O���Ȃ�
	//  ����1�s�������Ώ�Ƀz�[�~���O����
	}*/

	//�}�b�v�Ƃ̓����蔻��
	HitCheckMap();

	CMap* map = m_pOwner->GetMap();
	//�X�N���[���ʎ擾
	Math::Vector2 scrollPos = map->GetscrollPos();

	//�s��
	m_transMat = DirectX::XMMatrixTranslation(m_pos.x - scrollPos.x, m_pos.y - scrollPos.y, 0.0f);

	m_scaleMat = DirectX::XMMatrixScaling(m_scaleX, 1.0f, 0.0f);

	m_mat = m_scaleMat * m_transMat;	// �s��쐬
}

// �`��
void CEnemy::Draw()
{
	if (!m_bAlive) return;	// �t���O�m�F

	SHADER.m_spriteShader.SetMatrix(m_mat);	// �s��w�Z�b�g
	SHADER.m_spriteShader.DrawTex(m_pTexture, Math::Rectangle(0, 0, 64, 64), 1.0f);	// �`��
}

// �����m�F
const bool CEnemy::IsAlive()
{
	return m_bAlive;
}

// �e�N�X�`���󂯎��
void CEnemy::SetTexture(KdTexture* apTexture)
{
	if (apTexture == nullptr) return;	// �|�C���^�̃A�h���X�����킩���ׂ�(null�Ȃ�Z�b�g���Ȃ�)

	m_pTexture = apTexture;
}

//���W�擾
Math::Vector2 CEnemy::GetPos()
{
	return m_pos;
}

// ���E�ړ� 
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

// ���b�N�I��
void CEnemy::Homing()
{
	CPlayer player = m_pOwner->GetPlayer();		// �v���C���[�N���X�̎擾
	Math::Vector2 playerPos = player.GetPos();	// �v���C���[���W��playerPos�ɓ���

	deg = GetAngleDeg(m_pos,playerPos);

	m_moveVal.x = cos(DirectX::XMConvertToRadians(deg)) * 1.8;	// cos
	m_moveVal.y = sin(DirectX::XMConvertToRadians(deg)) * 1.8;	// sin
}

// ���W�ƈړ��ʂ��V�[������ݒ肷��֐�
void CEnemy::SetConfiguration(Math::Vector2 aPos, Math::Vector2 aMoveVal)
{
	m_pos = aPos;
	m_moveVal = aMoveVal;

}

float CEnemy::GetAngleDeg(Math::Vector2 src,Math::Vector2 dest)
{
	float a;	// ���
	float b;	// ����
	float rad;	// ���W�A��
	float deg;	// �f�O���[

	// �������Ԃɒ��ӁI�F�ړI�n - ���ŋ��߂�K�v������
	a = dest.x - src.x;
	b = dest.y - src.y;

	// tan�̋t�֐��Ŋp�x�����߂�  atan2(Y,X)�����̏��Ԃɒ���
	rad = atan2f(b, a);

	//�f�O���[�֕ϊ�(�v�Z���܂��͊֐����p)
	deg = DirectX::XMConvertToDegrees(rad);
	//deg = rad * 180.0f / 3.141592f;

	// ���̒l�̏ꍇ�͐��̒l�ɕ␳(����ŕK��0�`369�ɂȂ�)
	if (deg < 0)
	{
		deg += 360;
	}

	//�p�x��Ԃ�
	return deg;
}

float CEnemy::GetDistance(Math::Vector2 src, Math::Vector2 dest)
{
	float a, b, c;

	a = dest.x - src.x;			// ��ӂ����߂�
	b = dest.y - src.y;			// ���������߂�
	c = sqrt(a * a + b * b);	// �Εӂ����߂�<-���ꂪ��������
	return c;
}


void CEnemy::SetOwner(Scene* apOwner)
{
	if (apOwner == nullptr) return;

	m_pOwner = apOwner;
}

//�e�ɓ����������̏���
void CEnemy::HitBullet()
{
	m_bAlive = false;
}

//�}�b�v�Ƃ̓����蔻��
void CEnemy::HitCheckMap()
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
				//		�G�̃q�b�g�`�F�b�N								
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
