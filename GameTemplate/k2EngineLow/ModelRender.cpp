#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{
		//���C�g�̏�����
		//�f�B���N�V�������C�g
		m_light.dirLigDirection.x = 1.0f;
		m_light.dirLigDirection.y = -1.0f;
		m_light.dirLigDirection.z = -1.0f;

		m_light.dirLigDirection.Normalize();

		m_light.dirColor.x = 1.0f;
		m_light.dirColor.y = 1.0f;
		m_light.dirColor.z = 1.0f;

		//�A���r�G���g���C�g
		m_light.ambientLight.x = 0.3;
		m_light.ambientLight.y = 0.3;
		m_light.ambientLight.z = 0.3;

		//���_�̈ʒu
		m_light.eyePos = g_camera3D->GetPosition();
	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(const char* tkmfilePath)
	{
		ModelInitData initData;

		initData.m_tkmFilePath = tkmfilePath;
		initData.m_fxFilePath = "Assets/shader/testModel.fx";

		initData.m_expandConstantBuffer = &m_light;
		initData.m_expandConstantBufferSize = sizeof(m_light);

		//�쐬�����������f�[�^�����ƂɃ��f��������������A
		m_model.Init(initData);
	}

	void ModelRender::Update()
	{
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);
	}

	void ModelRender::SetPosition(Vector3 pos)
	{
		m_pos = pos;
	}

	void ModelRender::SetRotation(Quaternion rot)
	{
		m_rot = rot;
	}

	void ModelRender::SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
}