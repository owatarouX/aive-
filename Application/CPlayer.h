#pragma once

#include"CBullet.h"
#include"CSword.h"
#include"CBomb.h"

//�N���X�̑O���錾
class Scene;

enum eDirection	//�����Ă������
{
	Up,
	Down,
	Left,
	Right
};

//����Z���N�g
enum eClick
{
	eShuriken,	//0.�藠��
	eSword,		//1.��
	eBomb		//2.���e
};

//�藠���̗v�f��
constexpr int BULLET_MAX = 2;

//�v���C���[�N���X
class CPlayer
{
public:

	CPlayer();	//�R���X�g���N�^
	~CPlayer();	//�f�X�g���N�^

	//�v���C���[
	void Init();
	void Updata();
	void Draw();
	const bool IsAlive();

	//�Z�b�^�[
	void SetTexture(KdTexture* apTexture);
	void SetBulletTexture(KdTexture* apTexture);
	void SetSlashTexture(KdTexture* apTexture);
	void SetBombTexture(KdTexture* apTexture);
	void SetBlastTexture(KdTexture* apTexture);
	void SetOwner(Scene* apOwner);

	//�Q�b�^�[
	const Math::Vector2 GetPos();		//�v���C���[���W�擾
	const int GetHp();
	
	const int GetR();
	const int GetL();

private:		//�O������A�N�Z�X�s��

	eDirection		m_direction;
	eClick			m_LClick;
	eClick			m_RClick;

	KdTexture* m_pTexture;//�摜(�e�N�X�`��)
	Math::Vector2	 m_pos;		//���W
	Math::Vector2	 m_moveVal;	//�ړ���
	Math::Matrix	 m_mat;		//�s��
	Scene* m_pOwner;	//�I�[�i�[�擾�p
	
	bool			 m_bAlive;  //�����t���O
	int				 m_hp;		//HP
	int				 m_hpCount;	//���G����
	float			 m_alpha;	//�摜�̓����x
	bool			 m_HitFlg;	//�G�Ɠ�������������
	
	bool			 m_bRClick;	//�E�N���b�N�t���O
	bool			 m_bLClick;	//���N���b�N�t���O
	bool			 m_bRChange;//�E����ύX�t���O
	bool			 m_bLChange;//������ύX�t���O


	void HitCheckMap();			//�}�b�v�Ƃ̓����蔻��
	void HitCheckEnemy();		//�G�Ƃ̓����蔻��
	void HitCheckBomb();		//���e�Ƃ̓����蔻��
	void InviTime();			//���G����
	eClick ChangeItem(eClick click);		//����ύX�֐�

	CBullet m_bulletList[BULLET_MAX];
	void SetShuriken();
	CSword  m_swordList;
	void SetSword();
	CBomb  m_bombList;
	void SetBomb();

	
};