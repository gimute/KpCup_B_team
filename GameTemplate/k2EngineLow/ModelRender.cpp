#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{
		//ライトの初期化
		//ディレクションライト
		m_light.dirLigDirection.x = 1.0f;
		m_light.dirLigDirection.y = -1.0f;
		m_light.dirLigDirection.z = -1.0f;

		m_light.dirLigDirection.Normalize();

		m_light.dirColor.x = 1.0f;
		m_light.dirColor.y = 1.0f;
		m_light.dirColor.z = 1.0f;

		//アンビエントライト
		m_light.ambientLight.x = 0.3;
		m_light.ambientLight.y = 0.3;
		m_light.ambientLight.z = 0.3;

		//視点の位置
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

		//作成した初期化データをもとにモデルを初期化する、
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