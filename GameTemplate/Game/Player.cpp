#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Game.h"
#include "Enemy.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{

	m_animationclips[enAnimationClip_Idle].Load("Assets/modelData/player/proto_player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Walk].Load("Assets/modelData/player/proto_player/run.tka");
	m_animationclips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Crouching].Load("Assets/modelData/player/proto_player/crouching.tka");
	m_animationclips[enAnimationClip_Crouching].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Punch].Load("Assets/modelData/player/proto_player/punch.tka");
	m_animationclips[enAnimationClip_Punch].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Gunshot].Load("Assets/modelData/player/proto_player/gunshot.tka");
	m_animationclips[enAnimationClip_Gunshot].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/player/proto_player/proto_player2.tkm", m_animationclips, enAnimationClip_Num);

	m_charaCon.Init(25.0f, 40.0f, m_position);
	
	m_sphereCollider.Create(1.0f);

	m_game = FindGO<Game>("game");

	return true;
}

void Player::Update()
{
	//�ړ������B
	Move();
	//��]�����B
	Rotation();
	//�A�j���[�V��������
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ���
	ManageState();
	//m_modelRender.SetPosition(30.0f, 0.0f, 0.0f);
	//���f���̍X�V�B
	m_modelRender.Update();

}

void Player::Move()
{
	if (m_playerstate == enPlayerState_Attack)
		return;

	//xz�̈ړ����x��0.0f�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();
	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	right *= stickL.x * 200.0f;
	forward *= stickL.y * 200.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right + forward;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//���W��ݒ�B
	m_modelRender.SetPosition(m_position);

}

void Player::Rotation()
{
	if (m_playerstate == enPlayerState_Attack)
		return;

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//�G�`������ɉ�]��������B
		m_modelRender.SetRotation(m_rotation);
	}

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

}

void Player::AttackRotation()
{
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	Vector3 MinVec = m_forward * 500.0f;

	for (int ListnumA = 0; ListnumA < m_game->m_EnemyList.size(); ListnumA++) {
		Vector3 pos = m_game->m_EnemyList[ListnumA]->m_position;
		if (AngleCheck(pos)) {
			Vector3 diffA = pos - m_position;
			if (diffA.Length() <= MinVec.Length()){
				MinVec = diffA;
			}
		}
	}

	if (shot == false)
	{
		m_bullet = NewGO<Bullet>(0, "bullet");
		Quaternion rot;
		rot.SetRotation(Vector3::AxisZ, MinVec);
		m_bullet->SetMoveDirection(m_forward);
		m_bullet->Setrotation(rot);
		m_bullet->SetPosition(m_position);

		shot = true;
	}

	
}

struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& covexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����Ă��Ȃ�������
		if (covexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			//�Փ˂����͕̂ǂł͂Ȃ�
			return 0.0f;
		}

		//�ǂƂԂ�������t���O��true�ɂ���
		isHit = true;
		return 0.0f;
	}
};

bool Player::AngleCheck(const Vector3& position)
{
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	Vector3 diff = position - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));
	if (Math::PI * 0.35f <= fabsf(angle))
	{
		return false;
	}

	Vector3 EnemyPosition = position;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓v���C���[�̍��W
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//�I�_�̓G�l�~�[�̍��W
	end.setOrigin(btVector3(EnemyPosition.x, EnemyPosition.y + 70.0f, EnemyPosition.z));

	SweepResultWall callback;
	//���삵���R���C�_�[���n�_����I�_�܂œ������ĕǂɐڐG����������
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//�ǂƏՓ˂�����
	if (callback.isHit == true)
	{
		return false;
	}
	

	return true;
}

void Player::ManageState()
{
	switch (m_playerstate)
	{
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
	case Player::enPlayerState_Walk:
		ProcessWalkStateTransition();
		break;
	case Player::enPlayerState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void Player::PlayAnimation()
{
	switch (m_playerstate)
	{
	case Player::enPlayerState_Idle:
		//�ҋ@�A�j���[�V����
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case Player::enPlayerState_Walk:
		//����
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case Player::enPlayerState_Attack:
		//�U��
		m_modelRender.PlayAnimation(enAnimationClip_Gunshot, 0.1f);
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_playerstate = enPlayerState_Attack;
		return;
	}
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
		//�����X�e�[�g�ɂ���
		m_playerstate = enPlayerState_Walk;
		return;
	}
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else {
		//�ҋ@�X�e�[�g�ɂ���B
		m_playerstate = enPlayerState_Idle;
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	AttackRotation();
	if (m_modelRender.IsPlayingAnimation() == false )
	{
		ProcessCommonStateTransition();
		shot = false;
	}
	
}

void Player::Render(RenderContext& rc)
{
	//���f���̕`��B
	m_modelRender.Draw(rc);
}