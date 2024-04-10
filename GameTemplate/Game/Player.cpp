#include "stdafx.h"
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	m_modelRender.Init("Assets/modelData/player/proto_player/proto_player.tkm");

	m_charaCon.Init(25.0f, 75.0f, m_position);

	return true;
}

void Player::Update()
{
	//�ړ������B
	Move();
	//��]�����B
	Rotation();
	//�A�j���[�V�������Đ�����B
	//PlayAnimation();

	//m_modelRender.SetPosition(30.0f, 0.0f, 0.0f);
	//���f���̍X�V�B
	m_modelRender.Update();
}

void Player::Move()
{
	//xz�̈ړ����x��0.0f�ɂ���B
	//m_moveSpeed.x = 0.0f;
	//m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	//Vector3 stickL;
	//stickL.x = g_pad[0]->GetLStickXF();
	//stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	//Vector3 forward = g_camera3D->GetForward();
	//Vector3 right = g_camera3D->GetRight();
	////y�����ɂ͈ړ������Ȃ��B
	//forward.y = 0.0f;
	//right.y = 0.0f;

	////���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	//right *= stickL.x * 1000.0f;
	//forward *= stickL.y * 1000.0f;

	////�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	//m_moveSpeed += right + forward;

	//�n�ʂɕt���Ă�����B
	if (m_charaCon.IsOnGround())
	{
		//�d�͂𖳂����B
		m_moveSpeed.y = 0.0f;
		//LB�{�^���������ꂽ��B
		if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			//�W�����v������B
			m_moveSpeed.y = 300.0f;
		}
	}
	//�n�ʂɕt���Ă��Ȃ�������B
	else
	{
		//�d�͂𔭐�������B
		m_moveSpeed.y -= 15.5f;
	}
	
	//A�{�^�������ꂽ��B
	if (g_pad[0]->IsPress(enButtonA)) {
		//�X�s�[�h�オ���Ă����B
		if (m_speed < 600.0f) {
			m_speed += 3;
		}
		m_moveforward.Normalize();
		m_moveforward *= m_speed;
		//�A�N�Z�����Ƀu���[�L��������B
		if (g_pad[0]->IsPress(enButtonB)) {	
			m_speed *= 0.95;
			m_moveforward.Normalize();
			m_moveforward *= m_speed;
		}
	}
	//�A�N�Z�������Ȃ��Ńu���[�L��������B
	else if (g_pad[0]->IsPress(enButtonB)) {
		m_speed *= 0.95;
		m_moveforward.Normalize();
		m_moveforward *= m_speed;
	}
	//A�{�^��������Ȃ�������B
	else {
		//�X�s�[�h�����Ă����B
		if (m_speed >= 1) {
			m_speed -= 1;
			m_moveforward.Normalize();
			m_moveforward *= m_speed;
		}
	}
	m_position = m_charaCon.Execute(m_moveforward, g_gameTime->GetFrameDeltaTime());
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
	//if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	//{
	//	//�L�����N�^�[�̕�����ς���B
	//	m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	//	//�G�`������ɉ�]��������B
	//	m_modelRender.SetRotation(m_rotation);
	//}


	float Stick_y = g_pad[0]->GetLStickXF();

	m_rotation.AddRotationY(Stick_y /= 100);

	//��]��z�肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̕��ʃx�N�g�����v�Z����B
	m_moveforward = Vector3::AxisZ;
	m_rotation.Apply(m_moveforward);

}

void Player::Render(RenderContext& rc)
{
	//���f���̕`��B
	m_modelRender.Draw(rc);
}