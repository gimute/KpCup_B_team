#include "stdafx.h"
#include "Enemy.h"
#include "Game.h"
//#include "collision/CollisionObject.h"
#include "Player.h"

#define enemyspeed 150.0f                               //�ړ����x�̐��l
#define enemyattackspeed 300.0f                         //�U�����ړ����x�̐��l
#define enemyserch 500.0f * 500.0f						//�ǐՉ\�͈�
//#define enemyattack 

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
	//m_animationclips[enAnimationClip_Attack].Load("Assets/animData/Enemy/enemy_002/attack.tka");
	//m_animationclips[enAnimationClip_Attack].SetLoopFlag(true);
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

	//�R���W�����I�u�W�F�N�g���쐬����B
	//m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	//m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	//m_collisionObject->SetName("enemy_col");
	//m_collisionObject->SetPosition(m_position + corre1);
	////�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	//m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	

	return true;
}

void Enemy::Update()
{
	//�ǐՏ���
	Chase();
	//��]����
	Rotation();
	//�U������
	//Attack();
	//�����蔻�菈��
	//Collision();
	//�A�j���[�V����
	PlayAnimation();
	//�X�e�[�g�J�ڏ���
	ManageState();
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
	/*case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;*/

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
	/*case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	case enEnemyState_Down:
		ProcessDownStateTransition();
		break;*/
	}
}




void Enemy::ProcessChaseStateTransition()
{
	//�˒������ɓ�������
	if (SearchAttackDistance() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		m_attackTimer -= g_gameTime->GetFrameDeltaTime();
		ProcessCommonStateTransition();
		return;
	}
	ProcessCommonStateTransition();
}

void Enemy::ProcessAttackStateTransition()
{
	if (m_attackGotimer >= 0) {
		ProcessCommonStateTransition();
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
		//DeleteGO(m_attackcoll);
		m_attackcooltimer = attackcooltime;
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessIdleStateTransition()
{
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
	////�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	//if (IsAttack()) {
	//	return;
	//}
	//if (m_attackGotimer > 0) {
	//	m_attackGotimer -= g_gameTime->GetFrameDeltaTime();
	//}
	//�G�l�~�[���ړ�������B
	if (m_enemystate == enEnemyState_Idle)
		return;

	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		//�d�͂�0�ɂ���B
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//m_collisionObject->SetRotation(m_rotation);
	//m_collisionObject->SetPosition(m_position + corre1);
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	//���W��ݒ肷��B
	m_modelRender.SetPosition(modelPosition);
}

void Enemy::Rotation()
{
	//if (m_enemystate == enEnemyState_Attack) {
	//	return;
	//}
	if (fabsf(m_movespeed.x) < 0.001f
		&& fabsf(m_movespeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_movespeed.x, m_movespeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

//void Enemy::Attack()
//{
//	if (m_enemystate == enEnemyState_Attack) {
//		//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
//		m_attackcoll = NewGO<CollisionObject>(0);
//		//����̃R���W�������쐬����B
//		m_attackcoll->CreateSphere(m_position, Quaternion::Identity, 50.0f * m_scale.z);
//		m_attackcoll->SetName("enemy_attack");
//		Vector3 position = m_position + m_forward * 40.0f;
//		m_attackcoll->SetPosition(position + corre1);
//	}
//}

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
		//�p�x(��)��120��(����p)��菬������΁B
		if (angle <= (Math::PI / 180.0f) * 120.0f)
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
	return false;
}

const bool Enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�v���C���[�ɂ�����x�߂�������.�B

	if (diff.LengthSq() <= enemyserch)
	{
		//�v���C���[���˒������ɓ������I
		return true;
	}
	//�v���C���[���˒����O�������B
	return false;
}

void Enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B

	if (m_attackcooltimer > 0) {
		m_attackcooltimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//if (m_enemystate == enEnemyState_Attack) {
	//	return;
	//}
	Vector3 diff = m_player->GetPosition() - m_position;

	//�x�N�g���𐳋K������B
	//diff.Normalize();
	//�ړ����x��ݒ肷��B
	//m_movespeed = diff * enemyspeed;
	//�v���C���[����������
	if (SearchPlayer() == true)
	{
		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_movespeed = diff * enemyspeed;

		////�ǐՂł��鋗�����ǂ���
		//if (SearchChaseDistance() == true) {
		//	if(m_chaseTimer<0.0f)
		//}
		/*if (SearchAttackDistance()) {
			if (m_attackTimer < 0.0f) {
				m_enemystate = enEnemyState_Attack;
				m_attackTimer = attacktime;
				m_attackGotimer = attackGotime;
				m_movespeed = diff * enemyattackspeed;
				return;
			}
			if (m_enemystate == enEnemyState_Attack) {
				m_enemystate = enEnemyState_Chase;
				return;
			}*/
		m_enemystate = enEnemyState_Chase;
		return;
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
		return;
	}
}

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	
}