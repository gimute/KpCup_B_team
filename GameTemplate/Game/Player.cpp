#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"

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

	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	right *= stickL.x * 200.0f;
	forward *= stickL.y * 200.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right + forward;

	////�n�ʂɕt���Ă�����B
	//if (m_charaCon.IsOnGround())
	//{
	//	//�d�͂𖳂����B
	//	m_moveSpeed.y = 0.0f;
	//	//LB�{�^���������ꂽ��B
	//	if (g_pad[0]->IsTrigger(enButtonLB1))
	//	{
	//		//�W�����v������B
	//		m_moveSpeed.y = 300.0f;
	//	}
	//}
	////�n�ʂɕt���Ă��Ȃ�������B
	//else
	//{
	//	//�d�͂𔭐�������B
	//	m_moveSpeed.y -= 15.5f;
	//}
	//
	////A�{�^�������ꂽ��B
	//if (g_pad[0]->IsPress(enButtonA)) {
	//	//�X�s�[�h�オ���Ă����B
	//	if (m_speed < 600.0f) {
	//		m_speed += 3;
	//	}
	//	m_moveforward.Normalize();
	//	m_moveforward *= m_speed;
	//	//�A�N�Z�����Ƀu���[�L��������B
	//	if (g_pad[0]->IsPress(enButtonB)) {	
	//		m_speed *= 0.95;
	//		m_moveforward.Normalize();
	//		m_moveforward *= m_speed;
	//	}
	//}
	////�A�N�Z�������Ȃ��Ńu���[�L��������B
	//else if (g_pad[0]->IsPress(enButtonB)) {
	//	m_speed *= 0.95;
	//	m_moveforward.Normalize();
	//	m_moveforward *= m_speed;
	//}
	////A�{�^��������Ȃ�������B
	//else {
	//	//�X�s�[�h�����Ă����B
	//	if (m_speed >= 1) {
	//		m_speed -= 1;
	//		m_moveforward.Normalize();
	//		m_moveforward *= m_speed;
	//	}
	//}
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
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//�G�`������ɉ�]��������B
		m_modelRender.SetRotation(m_rotation);
	}

	//�̂̏���
	//float Stick_y = g_pad[0]->GetLStickXF();
	//m_rotation.AddRotationY(Stick_y /= 100);
	//m_modelRender.SetRotation(m_rotation);
	//m_moveforward = Vector3::AxisZ;
	//m_rotation.Apply(m_moveforward);

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

void Player::Render(RenderContext& rc)
{
	//���f���̕`��B
	m_modelRender.Draw(rc);
}