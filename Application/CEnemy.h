#pragma once

//�N���X�̑O���錾
class Scene;

class CEnemy
{
public:		// �O������A�N�Z�X��
	CEnemy();		// �R���X�g���N�^
	~CEnemy();		// �f�X�g���N�^

	void Init();			// ������
	void Update();			// �X�V
	void Draw();			// �`��
	const bool IsAlive();	// �����m�F
	void SetTexture(KdTexture* apTexture);	// �e�N�X�`���󂯎��
	Math::Vector2 GetPos();	// ���W�擾
	void Moveleftandright();// ���E�ړ� 
	void Homing();			// ���b�N�I��
	void SetConfiguration(
		Math::Vector2	aPos,
		Math::Vector2	aMoveVal);	// ���W�ƈړ��ʂ��V�[������ݒ肷��֐�
	float GetAngleDeg(Math::Vector2 src,
		Math::Vector2 dest);	// ���������߂�֐�
	float GetDistance(Math::Vector2 src,
		Math::Vector2 dest);// ���������߂�֐�

	void SetOwner(Scene* apOwner);
	void HitBullet();			//�e�ɓ����������̏���

private:	// �O������A�N�Z�X�s��
	KdTexture* m_pTexture;		// �摜(�e�N�X�`��)
	Math::Vector2	m_pos;		// ���W
	Math::Vector2	m_moveVal;	// �ړ���
	Math::Matrix	m_mat;		// �����s��
	Math::Matrix	m_transMat;	// �ړ��s��
	Math::Matrix	m_scaleMat;	// �g��s��
	float			m_scaleX;	// �g�嗦(�摜���]�p)
	int				m_count;	// �G�̏��
	bool			m_bAlive;	// �����t���O
	const float		m_radius;	// ���a
	float			deg;		// �p�x

	bool				HomingFlg;		// �z�[�~���O���邩���Ȃ��� 
	bool				VisibilityFlg;	// �~����ɓ�������?

	void HitCheckMap();		//�}�b�v�Ƃ̓����蔻��

	Scene* m_pOwner;	// �I�[�i�[�擾�p

};

