#include "stdafx.h"
#include "EnemyCamPos.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"

EnemyCamPos::EnemyCamPos()
{
	for (int i = 0; i < LIST_MAX; i++){
		m_camForwardList[i].m_num = i;
	}
}

EnemyCamPos::~EnemyCamPos()
{

}

float EnemyCamPos::EnemyCamPosConfirmation(Enemy* enemy)
{
	m_game = FindGO<Game>("game");
	m_camera = FindGO<GameCamera>("gamecamera");
	m_player = FindGO<Player>("player");

	//Enemy�̍��W���擾
	Vector3 enemypos = enemy->m_position;
	int Listnum = enemy->m_Vectornum;

		//�擾�������W���J�������ʂɂ��邩�ǂ����𒲂ׂ�
	if (AngleCheck(enemypos) /*&& EnemyStateIsAttack(Listnum)*/)
	{
		//�擾�������W���J�������ʂɂ������玟��
		//m_camForwardList�ɖ��o�^�̗v�f�����邩�ǂ����𒲂ׂ�
		if (ListCheck())
		{
			//���o�^�̏ꏊ������΂��̗v�f�ɍ��W��Enemy��o�^����
			Registration(Listnum);
		}
		else
		{
			//���o�^�̏ꏊ���Ȃ������ꍇ�͓o�^����Enemy�̒��ň��
			//�J�������牓��Enemy�𒲂ׂĂ���Enemy�����ݒT������Enemy����
			//�J��������߂��ꏊ�ɂ���Ȃ炻��m_camForwardList�̗v�f��Enemy��
			//�T������Enemy�Ɠ���ւ���
			CompareNear(Listnum, enemypos);
		}
	}
	else
	{
		//�擾�������W���J�������ʂɂȂ������ꍇ��
		//�擾�������W��Enemy�̍U���p�x��"��"�ɂ���
		CamOut(Listnum,enemypos);
	}

	if (enemy->m_enemyAttack == Enemy::en_FrequencyHigh)
	{
		return 3.0f;
	}
	else
	{
		return 10.0f;
	}
}

bool EnemyCamPos::ListCheck()
{
	//�\���̔z�����
	for (EnemyCamForward& ListPoint : m_camForwardList)
	{
		//�z��ɋ󂫂������
		if (ListPoint.m_enemyRegist == false)
		{
			//���̊J���Ă���z��̔ԍ���ϐ��ɓo�^����
			m_emptyPoint = ListPoint.m_num;
			//true��Ԃ�
			return true;
		}
	}

	//�J���Ă���ꏊ���������false��Ԃ�
	return false;
}

bool EnemyCamPos::ListCheck(int ListNum)
{
	//�\���̔z�����
	for (EnemyCamForward& ListPoint : m_camForwardList)
	{
		//�n���ꂽEnemy�̔z��ԍ��Ɠ����ԍ���
		//m_camForwardList�ɂ����
		if (ListPoint.m_registNum == ListNum)
		{
			//���̔z��̔ԍ���ϐ��ɓo�^����
			m_emptyPoint = ListPoint.m_num;
			//true��Ԃ�
			return true;
		}
	}

	//�J���Ă���ꏊ���������false��Ԃ�
	return false;
}

bool EnemyCamPos::EnemyStateIsAttack(int ListNum)
{
	if (m_game->m_EnemyList[ListNum]->m_enemystate
		== Enemy::enEnemyState_Attack)
	{
		return true;
	}
	return false;
}

bool EnemyCamPos::AngleCheck(const Vector3& position)
{
	Vector3 m_camForward = g_camera3D->GetForward();
	Vector3 m_camPosition = g_camera3D->GetPosition();

	Vector3 diff = position - m_camPosition;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_camForward));
	if (Math::PI * 0.15f <= fabsf(angle))
	{
		return false;
	}

	return true;
}

void EnemyCamPos::CompareNear(int ListNum,const Vector3& enemypos)
{
	//�J�����̌��ݍ��W���擾
	Vector3 CamPos = g_camera3D->GetPosition();
	//�����ȃx�N�g���ɂ���������y�l��0��
	CamPos.y = 0.0f;

	//�J�����̐��ʃx�N�g�����擾
	Vector3 CamForward = g_camera3D->GetForward();
	//�����ȃx�N�g��(�ȉ���)
	CamForward.y = 0.0f;
	//*400���ăx�N�g����L�΂�
	CamForward *= 400;

	//�ʒu+����*400�ŃJ�����̈ʒu���琳��*400�̍��W���o��
	Vector3 CamForwardAdvanceVec = CamPos + CamForward;

	//���ݒT������Enemy�̍��W���擾
	Vector3 changeEnemyPos = enemypos;

	//�擾����Enemy�̍��W�ƃJ��������*400�̌��ݍ��W���v�Z����
	//�v���C���[����Enemy�̍��W�֌������x�N�g�����o��
	Vector3 diffChange = changeEnemyPos - CamForwardAdvanceVec;

	//m_camForwardList�̒��łǂꂪ��ԃJ��������*400�̌��ݍ��W���牓��
	//�ʒu��Enemy���i�[���Ă��邩m_camForwardList�̗v�f�ԍ�
	//���擾����ϐ������
	int MaxListNum = -1;

	//��ׂ�p�ɕϐ����쐬
	Vector3 diffC;

	//m_camForwardList�̔z��̒��ň�ԉ���Enemy��o�^���Ă���v�f�ԍ��𒲂ׂ����̂�
	//for���̒���m_camForwardList��i��i + 1�ō��W���ׂ�
	//3��J��Ԃ��� + 1�񕪂��ӂ��̂�m_camForwardList�̑S�̗v�f������ - 1��J��Ԃ�
	for (int i = 0; i < LIST_MAX - 1; i++)
	{

		//m_camForwardList��i�̗v�f����o�^����Ă���Enemy�̍��W���擾����
		Vector3 compareEnemyA = m_game->GetEnemyListPos(m_camForwardList[i].m_registNum);

		//�擾����Enemy�̍��W�ƃJ��������*400�̌��ݍ��W���v�Z����
		//�J��������*400�̌��ݍ��W����Enemy�̍��W�֌������x�N�g�����o��
		Vector3 diffA = compareEnemyA - CamForwardAdvanceVec;

		//m_camForwardList��i + 1�̗v�f����o�^����Ă���Enemy�̍��W���擾����
		Vector3 compareEnemyB = m_game->GetEnemyListPos(m_camForwardList[i + 1].m_registNum);

		//�擾����Enemy�̍��W�ƃJ��������*400�̌��ݍ��W���v�Z����
		//�J��������*400�̌��ݍ��W����Enemy�̍��W�֌������x�N�g�����o��
		Vector3 diffB = compareEnemyB - CamForwardAdvanceVec;

		//�o���ꂽ2�̃x�N�g�����ׂĒ����x�N�g���̕���diffC�Ƃ���ׂ�
		//��������傫����΂��̗v�f�ԍ����i�[����
		if (diffA.Length() < diffB.Length())
		{
			if (diffC.Length() < diffB.Length())
			{
				MaxListNum = m_camForwardList[i + 1].m_num;
				diffC = diffB;
			}
		}
		else
		{
			if (diffC.Length() < diffA.Length())
			{
				MaxListNum = m_camForwardList[i].m_num;
				diffC = diffA;
			}
		}
	}

	if (diffC.Length() >= diffChange.Length())
	{
		if (!ListCheck(ListNum))
		{
			m_camForwardList[MaxListNum].m_registNum = ListNum;

			m_game->m_EnemyList[m_camForwardList[MaxListNum].m_registNum]
				->SetEnemyAttackState(Enemy::en_FrequencyHigh);
			return;
		}
	}
	else
	{
		m_game->m_EnemyList[ListNum]
			->SetEnemyAttackState(Enemy::en_FrequencyFew);
	}

	return;
}

void EnemyCamPos::CamOut(int ListNum,const Vector3 enemypos)
{
	//����m_camForwardList���ɓo�^����Ă���Enemy�̔z��ԍ���
	//���݂̒T���ꏊ�������Ȃ�o�^����������
	if (ListCheck(ListNum))
	{
		m_camForwardList[m_emptyPoint].m_registNum = -1;
		m_camForwardList[m_emptyPoint].m_enemyRegist = false;
	}

	if (!AngleCheck(enemypos))
	{
		//�U���p�x��"��"�ɂ��鏈��
		m_game->m_EnemyList[ListNum]->SetEnemyAttackState(Enemy::en_FrequencyFew);
	}
	return;
}

void EnemyCamPos::Registration(int ListNum)
{
	if (ListCheck(ListNum))
	{
		return;
	}

	m_camForwardList[m_emptyPoint].m_registNum = ListNum;
	m_camForwardList[m_emptyPoint].m_enemyRegist = true;
	m_game->m_EnemyList[ListNum]->SetEnemyAttackState(Enemy::en_FrequencyHigh);
}
