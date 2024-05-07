#include "stdafx.h"
#include "Enemy.h"
#include "EnemyAttackPoint.h"


namespace
{
	const float TO_ATTACK_POINT_DISTANSE = 200.0f;	//��ƂȂ���W����A�^�b�N�|�C���g�܂ł̋���
}

EnemyAttackPoint::EnemyAttackPoint()
{
	//�A�^�b�N�|�C���g�̃i���o�[������������
	for (int i = 0; i < ATTACK_POINT_NUM; i++)
	{
		m_AttackPointList[i].m_number = i;
	}
}

EnemyAttackPoint::~EnemyAttackPoint()
{

}

void EnemyAttackPoint::Update(Vector3 position)
{
	//�A�^�b�N�|�C���g���w�肳�ꂽ���W�̎��͂ɉ~��ɔz�u����

	//�|�C���g��ݒu�������
	Vector3 direction = g_vec3Front;

	//�|�C���g��ݒu�����������]������N�I�[�^�j�I��
	Quaternion directionRot = g_quatIdentity;

	//360�x���G�l�~�[�A�^�b�N�|�C���g�z��̗v�f���Ŋ����ē��Ԋu�Œu�����߂̊p�x��ݒ肷��
	directionRot.SetRotationDegY(360.0f / ATTACK_POINT_NUM);

	//�|�C���g�̔z����񂵂č��W��ݒ肷��
	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		//�|�C���g�̍��W��ݒ肷��
		attackPoint.m_position = position + direction * TO_ATTACK_POINT_DISTANSE;

		//�ݒu�����������]
		directionRot.Apply(direction);
	}
}

EnemyAttackPoint::AttackPoint* EnemyAttackPoint::GetNearAttackPoint(Vector3 position)
{
	//�A�^�b�N�|�C���g���g�p�\�łȂ����nullptr��Ԃ�
	if (IsUsableAttackPoint() == false)
	{
		return nullptr;
	}

	//������r�p�̕ϐ�
	float distance = 10000.0f;	//	�ŏ��͋ɒ[�ɑ傫���l�ɂ��Ă���

	//��ԋ߂��A�^�b�N�|�C���g�̃A�h���X�����Ă����|�C���^
	AttackPoint* tmp = nullptr;

	//�A�^�b�N�|�C���g�̔z�����
	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		//�A�^�b�N�|�C���g���g�p���Ȃ珈�����΂�
		if (attackPoint.m_use == true)
		{
			continue;
		}

		//�A�^�b�N�|�C���g�Ƃ̋��������߂�
		float compareDistance = (attackPoint.m_position - position).Length();

		//�ێ����Ă���A�^�b�N�|�C���g�Ƃ̋������A����o�����A�^�b�N�|�C���g�Ƃ̋����̕������������
		if (distance > compareDistance)
		{
			//�ێ����Ă���A�^�b�N�|�C���g�Ƃ̋������A����o�����A�^�b�N�|�C���g�Ƃ̋����ɕύX��
			distance = compareDistance;
			//���̃A�^�b�N�|�C���g�̃A�h���X��ۑ�����
			tmp = &attackPoint;
		}
	}

	//��ԋ߂��A�^�b�N�|�C���g�̃A�h���X��Ԃ�
	return tmp;
}

bool EnemyAttackPoint::IsUsableAttackPoint()
{
	//�A�b�N�|�C���g�g�p������萔��菭�Ȃ����
	if (m_useAttackPointNum < ATTACK_POINT_USE_LIMIT)
	{
		//�g�p�\
		return true;
	}
	else
	{
		//�g�p�s��
		return false;
	}
}

void EnemyAttackPoint::UseAttackPoint(int number, Enemy* enemy)
{
	//�|�C���g���g�p���Ȃ牽�������Ԃ��B
	if (m_AttackPointList[number].m_use == true)
	{
		return;
	}

	//�A�^�b�N�|�C���g���g�p���ɂ���B
	m_AttackPointList[number].m_use = true;
	//�A�^�b�N�|�C���g��ێ����Ă���G�l�~�[��o�^����
	m_AttackPointList[number].m_useEnemy = enemy;
	//�g�p���̃A�^�b�N�|�C���g�J�E���g�𑝂₷
	m_useAttackPointNum++;
}

void EnemyAttackPoint::ReleaseAttackPoint(int number, Enemy* enemy)
{
	//�|�C���g���m�ۂ��Ă���G�l�~�[�ƈ����œn���ꂽ�G�l�~�[�������łȂ���Ή��������Ԃ�
	if (m_AttackPointList[number].m_useEnemy != enemy)
	{
		return;
	}
	
	//�A�^�b�N�|�C���g�𖢎g�p�ɂ���B
	m_AttackPointList[number].m_use = false;
	//�A�^�b�N�|�C���g��ێ����Ă���G�l�~�[�̓o�^����������
	m_AttackPointList[number].m_useEnemy = nullptr;
	//�g�p���̃A�^�b�N�|�C���g�J�E���g�����炷
	m_useAttackPointNum--;
}

