#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	ModelRender::Light ModelRender::m_light;

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

	void ModelRender::Init(
		const char* tkmfilePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData initData;

		initData.m_tkmFilePath = tkmfilePath;
		initData.m_fxFilePath = "Assets/shader/testModel.fx";

		initData.m_expandConstantBuffer = &m_light;
		initData.m_expandConstantBufferSize = sizeof(m_light);

		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		initData.m_vsEntryPointFunc = "VSMain";
		//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		InitSkeleton(tkmfilePath);
		if (m_skeleton.IsInited())
		{
			initData.m_skeleton = &m_skeleton;
		}

		InitAnimation(animationClips, numAnimationClips);

		initData.m_modelUpAxis = enModelUpAxis;

		//�쐬�����������f�[�^�����ƂɃ��f��������������A
		m_model.Init(initData);
	}

	//tkm�t�@�C���̃p�X��.tkm�̕�����.tks�ɂ��ēǂݍ��ގ���
	//��������tkm�t�@�C���p�X��tks�t�@�C���p�X��������͂���K�v�������Ȃ�
	//�����tkm�t�@�C��������t�@�C����tks�t�@�C�����Ȃ��Ɠǂݍ��߂Ȃ��B
	void ModelRender::InitSkeleton(const char* tkmFilePath)
	{
		//�X�P���g���̃f�[�^��ǂݍ��݁B
		std::string skeletonFilePath = tkmFilePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr)
		{
			m_animation.Init(
				m_skeleton,
				m_animationClips,
				numAnimationClips //����Ȃ��m_numAnimationClips����Ȃ��񂾂�
			);
		}
	}

	void ModelRender::Update()
	{
		//���[���h�s��X�V
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);

		//�X�P���g�����������ς݂̏ꍇ�A�X�P���g���̍X�V
		if (m_skeleton.IsInited())
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//�A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());
	}
}
