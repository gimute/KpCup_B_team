#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h";

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(0.0f, 50.0f, -300.0f);
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ�B
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(30000.0f);

	return true;
}

void GameCamera::Update()
{
	////�����_���v�Z
	//Vector3 target = m_player->m_position;
	////�����_ �v���C���[�̍��W�ɐݒ�
	//target.y += 100.0f;

	//Vector3 playerBack = m_player->m_forward * -1;

	//Quaternion toPlayerBackRot;
	//toPlayerBackRot.SetRotation(m_toCameraPos, playerBack);

	//Vector3 tmp = m_toCameraPos;

	//toPlayerBackRot.Apply(tmp);

	//Vector3 pos = target + tmp;

	////���C���J�����ɒ����_�Ǝ��_��ݒ�
	//g_camera3D->SetTarget(target);
	//g_camera3D->SetPosition(pos);

	////�J�����̍X�V
	//g_camera3D->Update();

	///////////////////////////////////////////////////////////
	//�J�������X�V
	//�����_���v�Z
	Vector3 target = m_player->m_position;
	//�����_ �v���C���[�̍��W�ɐݒ�
	target.y += 100.0f;
	//���_���v�Z����B
	Vector3 toCameraPosOld = m_toCameraPos;

	//�p�b�h�̓��͂��g���ăJ��������
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);
	//X������̉�]
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);
	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	//�傫�����P�ɂȂ遨�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ�������
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.2f) {
		//�J���������������
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//�J����������������
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;

	

	//���C���J�����ɒ����_�Ǝ��_��ݒ�
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//�J�����̍X�V
	g_camera3D->Update();
}