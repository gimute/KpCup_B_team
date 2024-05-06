#include "stdafx.h"
#include "Enemy.h"
#include "EnemyAttackPoint.h"

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

	//��̍��W����|�C���g�܂ł̋���
	float lenge = 200.0f;

	//�|�C���g��ݒu�������
	Vector3 direction = g_vec3Front;

	//�|�C���g��ݒu�����������]������N�I�[�^�j�I��
	Quaternion directionRot = g_quatIdentity;

	//360�x���G�l�~�[�A�^�b�N�|�C���g�z��̗v�f���Ŋ����ē��Ԋu�Œu�����߂̊p�x��ݒ肷��
	directionRot.SetRotationDegY(360.0f / ENEMY_ATTACK_POINT_NUM);

	//�|�C���g�̔z����񂵂č��W��ݒ肷��
	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		//�|�C���g�̍��W��ݒ肷��
		attackPoint.m_position = position + direction * lenge;

		//�ݒu�����������]
		directionRot.Apply(direction);
	}
}

EnemyAttackPoint::AttackPoint* EnemyAttackPoint::GetNearAttackPoint(Vector3 position)
{
	//�A�^�b�N�|�C���g���g�p�\�łȂ����nullptr��Ԃ�
	if (IsUsableAttackPoint())
	{
		return nullptr;
	}

	//������r�p�̕ϐ�
	float distance = 10000.0f;	//	�ŏ��͋ɒ[�ɑ傫���l�ɂ��Ă���

	//��ԋ߂��A�^�b�N�|�C���g�̃A�h���X�����Ă����|�C���^
	AttackPoint* tmp = nullptr;

	//�g�p���̃A�^�b�N�|�C���g�̐����J�E���g����ϐ�
	int useCount = 0;


	for (AttackPoint& attackPoint : m_AttackPointList)
	{
		if (attackPoint.m_use == true)
		{
			continue;
		}

		float distance2 = (attackPoint.m_position - position).Length();

		//diff�̒l���A�^�b�N�|�C���g�Ƃ̋����̕������������
		if (distance > distance2)
		{
			//diff�̒l���߂����̃A�^�b�N�|�C���g�Ƃ̋����ɕύX��
			distance = distance2;
			//���̃A�^�b�N�|�C���g�̃A�h���X��ۑ�����
			tmp = &attackPoint;
		}
	}

	//��ԋ߂��A�^�b�N�|�C���g�̃A�h���X��Ԃ�
	return tmp;
}

bool EnemyAttackPoint::IsUsableAttackPoint()
{
	if (m_useAttackPointNum <= 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void EnemyAttackPoint::GetAttackPoint(int number, Enemy* enemy)
{
	//�|�C���g���g�p���Ȃ牽�������Ԃ�
	if (m_AttackPointList[number].m_use == true)
	{
		return;
	}

	m_AttackPointList[number].m_use = true;
	m_AttackPointList[number].m_useEnemy = enemy;

	m_useAttackPointNum++;
}

void EnemyAttackPoint::ReleaseAttackPoint(int number, Enemy* enemy)
{
	//�|�C���g���m�ۂ��Ă���G�l�~�[�ƈ����œn���ꂽ�G�l�~�[�������łȂ���Ή��������Ԃ�
	if (m_AttackPointList[number].m_useEnemy != enemy)
	{
		return;
	}
	
	m_AttackPointList[number].m_use = false;
	m_AttackPointList[number].m_useEnemy = nullptr;

	m_useAttackPointNum--;
}

//Vector3 EnemyAttackPoint::GetNearAttackPointPosition()
//{
//	//������r�p�̕ϐ�
//	float distance = 10000.0f;	//	�ŏ��͋ɒ[�ɑ傫���l�ɂ��Ă���
//
//	//��ԋ߂��A�^�b�N�|�C���g�̃A�h���X�����Ă����|�C���^
//	EnemyAttackPoint* tmp = nullptr;
//
//	//�g�p���̃A�^�b�N�|�C���g�̐����J�E���g����ϐ�
//	int useCount = 0;
//
//	//��ԋ߂����g�p�̃A�^�b�N�|�C���g��T��
//	for (int i = 0; i < ENEMY_ATTACK_POINT_NUM; i++)
//	{
//		//�A�^�b�N�|�C���g���g�p���Ȃ�
//		if (m_enemyAttackPointList[i].m_use == true)
//		{
//			//�g�p���A�^�b�N�|�C���g�̃J�E���g�𑝂₷
//			useCount++;
//			//�g�p���̃|�C���g����萔�𒴂��Ă��邱�Ƃ��킩������
//			if (useCount >= 5)
//			{
//				//nullptr��Ԃ�
//				return nullptr;
//			}
//			//�����ĂȂ����
//			else
//			{
//				continue;
//			}
//
//		}
//
//		//diff�̒l���A�^�b�N�|�C���g�Ƃ̋����̕������������
//		if (diff > (m_enemyAttackPointList[i].m_position - pos).Length())
//		{
//			//diff�̒l���߂����̃A�^�b�N�|�C���g�Ƃ̋����ɕύX��
//			diff = (m_enemyAttackPointList[i].m_position - pos).Length();
//			//���̃A�^�b�N�|�C���g�̃A�h���X��ۑ�����
//			tmp = &m_enemyAttackPointList[i];
//		}
//	}
//
//	//��ԋ߂��A�^�b�N�|�C���g�̃A�h���X��Ԃ�
//	return tmp;
//}

