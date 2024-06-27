#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {


	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(
		const char* tkmfilePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool dithering,
		bool isRecieveShadow
	)
	{
		//�X�P���g���̏�����
		InitSkeleton(tkmfilePath);

		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips);

		//��{�̃��f��������
		InitNormalModel(tkmfilePath, animationClips, numAnimationClips, enModelUpAxis, dithering, isRecieveShadow);

		//�V���h�E�}�b�v�`��p���f���̏�����
		InitShadowModel(tkmfilePath, enModelUpAxis);

	}

	void ModelRender::InitNormalModel(
		const char* tkmfilePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool dithering,
		bool isRecieveShadow
	)
	{
		ModelInitData initData;

		initData.m_tkmFilePath = tkmfilePath;

		if (dithering == false)
		{
			initData.m_fxFilePath = "Assets/shader/model.fx";
		}
		else
		{
			initData.m_fxFilePath = "Assets/shader/ditheringModel.fx";
		}

		if (isRecieveShadow == true)
		{
			initData.m_fxFilePath = "Assets/shader/shadowReceiverModel.fx";


			Camera lightCamera;

			// �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
			lightCamera.SetPosition(0, 2000, 0);

			// �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
			lightCamera.SetTarget(0, 0, 0);

			// �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
			lightCamera.SetUp(1, 0, 0);

			// ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
			lightCamera.Update();

			m_tsts.m_light = *g_sceneLight->GetLightData();
			m_tsts.m_mt = lightCamera.GetViewProjectionMatrix();

			initData.m_expandConstantBuffer = &m_tsts;
			initData.m_expandConstantBufferSize = sizeof(m_tsts);

			//�V���h�E�}�b�v���g��SRV�ɐݒ肷��
			initData.m_expandShaderResoruceView[0] = &(g_renderingEngine->GetShadowMapRenderTarget()->GetRenderTargetTexture());
		}
		else
		{
			initData.m_expandConstantBuffer = g_sceneLight->GetLightData();
			initData.m_expandConstantBufferSize = sizeof(Light);
		}

		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		initData.m_vsEntryPointFunc = "VSMain";
		//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//�X�P���g��������������Ă�����
		if (m_skeleton.IsInited())
		{
			initData.m_skeleton = &m_skeleton;
		}

		initData.m_modelUpAxis = enModelUpAxis;

		//�쐬�����������f�[�^�����ƂɃ��f��������������A
		m_model.Init(initData);
	}

	void ModelRender::InitShadowModel(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
	{
		ModelInitData initData;
		initData.m_tkmFilePath = tkmFilePath;
		initData.m_modelUpAxis = modelUpAxis;

		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		//initData.m_vsEntryPointFunc = "VSMain";
		//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
		//initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			initData.m_skeleton = &m_skeleton;
		}

		initData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";

		m_shadowModel.Init(initData);
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

		m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_scale);

		//�X�P���g�����������ς݂̏ꍇ�A�X�P���g���̍X�V
		if (m_skeleton.IsInited())
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//�A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddRenderObject(this);
	}

	void ModelRender::OnRenderModel(RenderContext& rc)
	{
		m_model.Draw(rc);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)
	{
		if (m_isShadowChaster)
		{
			m_shadowModel.Draw(
				rc,
				g_matIdentity,
				lvpMatrix,
				1
			);
		}
	}
}
