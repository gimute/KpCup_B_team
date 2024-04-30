#include "stdafx.h"
#include "Enemy.h"
#include "Game.h"
//#include "collision/CollisionObject.h"
#include "Player.h"

#define enemyspeed 150.0f                               //�ړ����x�̐��l
#define enemyattackspeed 300.0f                         //�U�����ړ����x�̐��l
#define enemyserch 500.0f * 500.0f						//�ǐՉ\�͈�
#define enemyattack 300.0f * 300.0f						//�U���\�͈�

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//�ʒu�C��
	const Vector3 corre2 = { 2.0f,2.0f,2.0f };
}

bool Enemy::Start()
{

	//�A�j���[�V�����ǂݍ���
	m_animationclips[enAnimationClip_Idle].Load("Assets/modelData/player/proto_player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/modelData/player/proto_player/run.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Attack].Load("Assets/modelData/player/proto_player/gunshot.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(true);//
	//m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Enemy/enemy_002/receivedamage.tka");
	//m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	//m_animationclips[enAnimationClip_Down].Load("Assets/animData/Enemy/enemy_002/down.tka");
	//m_animationclips[enAnimationClip_Down].SetLoopFlag(false);


	//���f���ǂݍ���
	m_modelRender.Init("Assets/modelData/player/proto_player/proto_player2.tkm", m_animationclips, enAnimationClip_Num);

	//��]
	m_modelRender.SetRotation(m_rotation);
	//���W
	m_modelRender.SetPosition(m_position);
	//�X�P�[��
	m_modelRender.SetScale(m_scale);
	//�L�����R��������
	m_charaCon.Init(20.0f, 0.0f, m_position);

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	

	return true;
}

void Enemy::Update()
{
	//�X�e�[�g�J�ڏ���
	ManageState();
	
	switch (m_enemystate)
	{
	case enEnemyState_Idle:

		break;

	case enEnemyState_Chase:
		//�ǐՏ���
		Chase();
		break;

	case enEnemyState_Attack:
		//�U������
		Attack();
		break;
	}

	//��]����
	Rotation();
	
	//�����蔻�菈��
	//Collision();
	//�A�j���[�V����
	PlayAnimation();
	
	
	//�`��X�V
	m_modelRender.Update();
}

void Enemy::PlayAnimation()
{
	//m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
		//�ҋ@
	case enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Chase, 0.1f);
		break;
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	}
}

void Enemy::ManageState()
{
	switch (m_enemystate)
	{
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}




void Enemy::ProcessChaseStateTransition()
{
	// �v���C���[���U���͈͓��ɓ�������
	if (SearchAttackDistance() == true)
	{
		// ���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	// �v���C���[���܂��߂��ɂ���Ȃ�
	else if (SearchChaseDistance())
	{
		// �X�e�[�g��ύX�����Ԃ��B
		return;
	}
	// �v���C���[���߂��ɂ��Ȃ��Ȃ�
	else
	{
		// ���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
	
}

void Enemy::ProcessAttackStateTransition()
{
	// ���̂Ƃ���͂��ł����̃X�e�[�g�ɑJ�ډ\
	// ��Ń��[�V�������I������܂œ��̏�����ǉ�
	ProcessCommonStateTransition();
}

void Enemy::ProcessIdleStateTransition()
{
	// �ҋ@��Ԃ���͂��ł����̃X�e�[�g�ɑJ�ډ\
	ProcessCommonStateTransition();
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{
	//	ProcessCommonStateTransition();
	//}
}

void Enemy::ProcessDownStateTransition()
{
	////��_���[�W�A�j���[�V�����̍Đ����I�������B
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{
	//	SoundSource* se = NewGO<SoundSource>(11);
	//	se = NewGO<SoundSource>(11);
	//	se->Init(11);
	//	se->Play(false);
	//	ItemDrop();
	//	DeleteGoEnemyList();
	//	DeleteGO(m_collisionObject);//��������
	//	DeleteGO(this);
	//}
}

void Enemy::Chase()
{
	//�O�̂��߃X�e�[�g�m�F
	if (m_enemystate != enEnemyState_Chase)
	{
		return;
	}

	//�v���C���[�Ɍ������x�N�g�����߂�
	Vector3 diff = m_player->GetPosition() - m_position;
	
	diff.Normalize();

	m_movespeed = diff * enemyspeed;

	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
}

void Enemy::Rotation()
{
	// �v���C���[�Ɍ������x�N�g�������߂�B
	Vector3 diff = m_player->GetPosition() - m_position;

	// �X�e�[�g�ɂ���ď�����ς���
	switch (m_enemystate)
	{
	case Enemy::enEnemyState_Idle:
		break;

		//�ǐՃX�e�[�g�̏ꍇ///////////////////////////////////////////////////////
		//�i�s�����Ɍ�������
	case Enemy::enEnemyState_Chase:
		if (fabsf(m_movespeed.x) < 0.001f
			&& fabsf(m_movespeed.z) < 0.001f) {
			//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
			//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
			return;
		}

		//movespeed����G�l�~�[�̌��������߂�
		m_rotation.SetRotationY(-atan2(-m_movespeed.x, m_movespeed.z));

		//��]��ݒ肷��B
		m_modelRender.SetRotation(m_rotation);

		//�G�l�~�[�̐��ʕ����x�N�g�������߂�
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		break;
		///////////////////////////////////////////////////////////////////////////

		//�A�^�b�N�X�e�[�g�̏ꍇ///////////////////////////////////////////////////
		//�v���C���[�̂�������Ɍ�������
	case Enemy::enEnemyState_Attack:
		//���f���̐��ʕ���(z�������ɐL�т�P�ʃx�N�g��)����A�v���C���[�Ɍ������x�N�g�������ɉ�]������N�I�[�^�j�I�����쐬�B
		m_rotation.SetRotation(Vector3::AxisZ, diff);
		//�쐬�����N�I�[�^�j�I�������f���̃��[�e�[�V�����ɓK���B
		m_modelRender.SetRotation(m_rotation);

		//�G�l�~�[�̐��ʕ����x�N�g�������߂�
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		break;
		//////////////////////////////////////////////////////////////////////////

	default:
		break;
	}
}

void Enemy::Attack()
{
	//�O�̂��߃X�e�[�g�m�F
	if (m_enemystate != enEnemyState_Attack)
	{
		return;
	}

	//�U������
	//�e��΂�����Ƃ�����
}

//void Enemy::Collision()
//{
//	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
////�����蔻�菈���͂��Ȃ��B
//	if (m_enemystate == enEnemyState_ReceiveDamage ||
//		m_enemystate == enEnemyState_Down)
//	{
//		return;
//	}
//	//���G���Ԓ��͏������Ȃ�
//	if (m_mutekitimer > 0)
//	{
//		m_mutekitimer -= g_gameTime->GetFrameDeltaTime();
//		return;
//	}
//	//�v���C���[�̍U���p�̃R���W�������擾����B
//	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
//	//�R���W�����̔z���for���ŉ񂷁B
//	for (auto collision : collisions)
//	{
//		//�R���W�����ƃL�����R�����Փ˂�����B
//		if (collision->IsHit(m_collisionObject))
//		{
//			SoundSource* se = NewGO<SoundSource>(5);
//			se = NewGO<SoundSource>(5);
//			se->Init(5);
//			se->Play(false);
//			if (m_sh > 0) {
//				m_sh--;
//				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
//				m_enemystate = enEnemyState_ReceiveDamage;
//				return;
//			}
//			else {
//				//HP��1���炷�B
//				m_hp -= 1;
//				m_mutekitimer = mutekitime;
//				//HP��0�ɂȂ�����B
//				if (m_hp == 0) {
//					//�_�E���X�e�[�g�ɑJ�ڂ���B
//					m_enemystate = enEnemyState_Down;
//				}
//				else {
//					//��_���[�W�X�e�[�g�ɑJ�ڂ���B
//					m_enemystate = enEnemyState_ReceiveDamage;
//				}
//			}
//		}
//	}
//}


const bool Enemy::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������.�B
	if (diff.LengthSq() <= enemyserch)
	{
		// �G����v���C���[�Ɍ������x�N�g���𐳋K������
		diff.Normalize();
		//�G�̐��ʂ̃x�N�g���ƁA�G����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff);
		if (cos >= 1)
		{
			cos = 1.0f;
		}
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��90��(����p)��菬������΁B
		if (angle <= (Math::PI / 180.0f) * 90.0f)
		{
			//�v���C���[���������I
			return true;
		}
	}
	//�v���C���[���������Ȃ������B
	return false;
}
const bool Enemy::SearchChaseDistance() const
{
	//�v���C���[�Ɍ������x�N�g�����v�Z
	Vector3 diff = m_player->GetPosition() - m_position;
	//���߂��x�N�g���̋����������Z�����
	if (diff.Length() <= 300.0f)
	{
		//�v���C���[���߂��ɂ���
		return true;
	}
	//�v���C���[���߂��ɂ��Ȃ�
	return false;
}

const bool Enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�v���C���[�ɂ�����x�߂�������B

	if (diff.LengthSq() <= enemyattack)
	{
		//�v���C���[���˒������ɓ������I
		return true;
	}
	//�v���C���[���˒����O�������B
	return false;
}

void Enemy::ProcessCommonStateTransition()
{
	//�v���C���[�����E���ɋ��邩�A
	if (SearchPlayer())
	{
		//�U���ł��鋗���Ȃ�
		if (SearchAttackDistance())
		{
			//�X�e�[�g���A�^�b�N�ɂ���B
			m_enemystate = enEnemyState_Attack;
		}
		//���E�������U���͈͓��ł͂Ȃ��Ȃ�
		else
		{
			//�X�e�[�g��ǐՂɂ���B
			m_enemystate = enEnemyState_Chase;
		}
	}
	//�v���C���[�����E���ɋ��Ȃ��Ȃ�
	else
	{
		//�X�e�[�g��ҋ@�ɂ���
		m_enemystate = enEnemyState_Idle;
	}
}

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	
}