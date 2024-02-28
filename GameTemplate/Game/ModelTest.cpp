#include "stdafx.h"
#include "ModelTest.h"

bool ModelTest::Start()
{
	//���C�g�̏�����///////////////////////

	//�f�B���N�V�������C�g
	m_light.dirLigDirection.x = 1.0f;
	m_light.dirLigDirection.y = -1.0f;
	m_light.dirLigDirection.z = -1.0f;

	m_light.dirLigDirection.Normalize();

	m_light.dirColor.x = 1.0f;
	m_light.dirColor.y = 1.0f;
	m_light.dirColor.z = 1.0f;

	m_light.eyePos = g_camera3D->GetPosition();

	//�|�C���g���C�g
	m_light.ptPosition.x = 0.0f;
	m_light.ptPosition.y = 50.0f;
	m_light.ptPosition.z = 0.0f;

	m_light.ptColor.x = 15.0f;
	m_light.ptColor.y = 0.0f;
	m_light.ptColor.z = 0.0f;

	m_light.ptRange = 100.0f;

	//�X�|�b�g���C�g
	m_light.spPosition.x = 0.0f;
	m_light.spPosition.y = 50.0f;
	m_light.spPosition.z = 0.0f;

	m_light.spColor.x = 10.0f;
	m_light.spColor.y = 10.0f;
	m_light.spColor.z = 10.0f;

	m_light.spDirection.x = 1.0f;
	m_light.spDirection.y = -1.0;
	m_light.spDirection.z = 1.0f;
	m_light.spDirection.Normalize();

	m_light.spRange = 300.0f;

	m_light.spAngle = Math::DegToRad(25.0f);

	//�������C�g
	m_light.groundColor.x = 0.7f;
	m_light.groundColor.y = 0.5f;
	m_light.groundColor.z = 0.3f;

	m_light.skyColor.x = 0.15f;
	m_light.skyColor.y = 0.7f;
	m_light.skyColor.z = 0.95f;

	m_light.groundNormal.x = 0.0f;
	m_light.groundNormal.y = 1.0f;
	m_light.groundNormal.z = 0.0f;
	//�ꉞ���K��
	m_light.groundNormal.Normalize();

	////////////////////////////////////

	// ���f�������������邽�߂̏���ݒ�B
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/test.fx";

	initData.m_expandConstantBuffer = &m_light;
	initData.m_expandConstantBufferSize = sizeof(m_light);

	//�쐬�����������f�[�^�����ƂɃ��f��������������A
	m_model.Init(initData);


	//bg���f��
	ModelInitData bgModelInitData;
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg.tkm";
	bgModelInitData.m_fxFilePath = "Assets/shader/test.fx";

	bgModelInitData.m_expandConstantBuffer = &m_light;
	bgModelInitData.m_expandConstantBufferSize = sizeof(m_light);

	m_bgModel.Init(bgModelInitData);

	return true;
}

void ModelTest::Update()
{

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_moveState++;

		m_moveState %= 4;
	}

	switch (m_moveState)
	{
	case 0:
		ModelMove();
		break;

	case 1:
		DirLigMove();
		break;

	case 2:
		PtLigMove();
		break;

	case 3:
		SpotLigMove();
		break;

	default:
		break;
	}

}

void ModelTest::Render(RenderContext& rc)
{
	m_model.Draw(rc);
	m_bgModel.Draw(rc);
}

void ModelTest::ModelMove()
{
	//���f���̈ړ�
	m_position.x += g_pad[0]->GetLStickXF();
	m_position.y += g_pad[0]->GetLStickYF();
	//���f���̉�]
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	//�ǂ�����J������̏����ɂ��Ă��Ȃ�
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}

	//���f���̃��[���h�s��X�V
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void ModelTest::DirLigMove()
{
	//�f�B���N�V�������C�g�̌����̉�]
	Quaternion qRot;
	qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
	qRot.Apply(m_light.dirLigDirection);
	qRot.SetRotationDegX(g_pad[0]->GetLStickYF());
	qRot.Apply(m_light.dirLigDirection);
}

void ModelTest::PtLigMove()
{
	m_light.ptPosition.x -= g_pad[0]->GetLStickXF();
	if (g_pad[0]->IsPress(enButtonB))
	{
		m_light.ptPosition.y += g_pad[0]->GetLStickYF();
	}
	else
	{
		m_light.ptPosition.z -= g_pad[0]->GetLStickYF();
	}
}

void ModelTest::SpotLigMove()
{
	m_light.spPosition.x -= g_pad[0]->GetLStickXF();
	if (g_pad[0]->IsPress(enButtonB))
	{
		//B�{�^�����ꏏ�ɉ�����Ă�����Y�������ɓ�����
		m_light.spPosition.y += g_pad[0]->GetLStickYF();
	}
	else
	{
		//Z�������ɓ�����
		m_light.spPosition.z -= g_pad[0]->GetLStickYF();
	}

	// step-4 �R���g���[���[�E�X�e�B�b�N�ŃX�|�b�g���C�g����]������
	//Y������̉�]�N�H�[�^�j�I�����v�Z����
	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);

	//�v�Z�����N�H�[�^�j�I���f���C�g�̕�������
	qRotY.Apply(m_light.spDirection);

	//X������̉�]�N�H�[�^�j�I�����v�Z����
	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, m_light.spDirection);
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);

	//�v�Z�����N�H�[�^�j�I���f���C�g�̕�������
	qRotX.Apply(m_light.spDirection);
}
