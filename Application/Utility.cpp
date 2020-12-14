#include "Utility.h"

//�l�ӂ̓����蔻��`�F�b�N
//����������true��false��Ԃ�
bool Utility::bHitCheck(Math::Vector2 aScrPos, Math::Vector2 aScrMove, Math::Vector2 aDestPos, float aScrRad, float aDesRad)
{
	//�L����1�̌��ݍ��W�̎l��
	const float aScrLeft = aScrPos.x - Infor::RADIUS_32;		//����
	const float aScrRight = aScrPos.x + Infor::RADIUS_32;		//�E��
	const float aScrTop = aScrPos.y + Infor::RADIUS_32;		//���
	const float aScrBottom = aScrPos.y - Infor::RADIUS_32;		//����

	//�L����1�̖������W�̎l��
	const float nextLeft = aScrLeft + aScrMove.x;				//����
	const float nextRight = aScrRight + aScrMove.x;			//�E��
	const float nextTop = aScrTop + aScrMove.y;				//���
	const float nextBottom = aScrBottom + aScrMove.y;			//����

	//�L����2�̌��ݍ��W�̎l��
	const float aDestLeft = aDestPos.x - Infor::RADIUS_32;	//����
	const float aDestRight = aDestPos.x + Infor::RADIUS_32;	//�E��
	const float aDestTop = aDestPos.y + Infor::RADIUS_32;	//���
	const float aDestBottom = aDestPos.y - Infor::RADIUS_32;	//����

	/* ���ݍ��W :���E�����蔻��*/
	if (aScrRight > aDestLeft && aScrLeft < aDestRight)
	{
		//�������W :�ォ�炠������������
		if (nextBottom < aDestTop && nextTop > aDestTop)
		{
			return false;
		}
		//�������W :�����炠������������
		else if (aScrTop > aDestBottom && aScrBottom < aDestBottom)
		{
			return false;
		}
	}

	/* ���ݍ��W :�㉺�����蔻��*/
	if (aScrTop > aDestBottom && aScrBottom < aDestTop)
	{
		//�������W :�����炠������������
		if (nextRight > aDestLeft && nextLeft < aDestLeft)
		{
			return false;
		}
		//�������W :�E���炠������������
		else if (nextLeft < aDestRight && nextRight > aDestRight)
		{
			return false;
		}
	}
}

//�l�ӂ̓����蔻����X�ɔ���
//1:�㔻�� 2:������ 3:�E 4:��
int Utility::iHitCheck(Math::Vector2 aScrPos, Math::Vector2 aScrMove, float aDestPosX, float aDestPosY, float aScrRad, float aDesRad)
{
	//�L����1�̌��ݍ��W�̎l��
	const float aScrLeft = aScrPos.x - Infor::RADIUS_32;		//����
	const float aScrRight = aScrPos.x + Infor::RADIUS_32;		//�E��
	const float aScrTop = aScrPos.y + Infor::RADIUS_32;		//���
	const float aScrBottom = aScrPos.y - Infor::RADIUS_32;		//����

	//�L����1�̖������W�̎l��
	const float nextLeft = aScrLeft + aScrMove.x;				//����
	const float nextRight = aScrRight + aScrMove.x;			//�E��
	const float nextTop = aScrTop + aScrMove.y;				//���
	const float nextBottom = aScrBottom + aScrMove.y;			//����

	//�L����2�̌��ݍ��W�̎l��
	const float aDestLeft = aDestPosX - Infor::RADIUS_32;	//����
	const float aDestRight = aDestPosX + Infor::RADIUS_32;	//�E��
	const float aDestTop = aDestPosY + Infor::RADIUS_32;	//���
	const float aDestBottom = aDestPosY - Infor::RADIUS_32;	//����

	/* ���ݍ��W :���E�����蔻��*/
	if (aScrRight > aDestLeft && aScrLeft < aDestRight)
	{
		//�������W :�ォ�炠������������
		if (nextBottom < aDestTop && nextTop > aDestTop)
		{
			return 1;	//��ɉ����߂�
		}
		//�������W :�����炠������������
		else if (aScrTop > aDestBottom && aScrBottom < aDestBottom)
		{
			return 2;	//���ɉ����߂�
		}
	}

	/* ���ݍ��W :�㉺�����蔻��*/
	if (aScrTop > aDestBottom && aScrBottom < aDestTop)
	{
		//�������W :�����炠������������
		if (nextRight > aDestLeft && nextLeft < aDestLeft)
		{
			return 3;	//���ɉ����߂�
		}
		//�������W :�E���炠������������
		else if (nextLeft < aDestRight && nextRight > aDestRight)
		{
			return 4;	//�E�ɉ����߂�
		}
	}

	return 0;
}

