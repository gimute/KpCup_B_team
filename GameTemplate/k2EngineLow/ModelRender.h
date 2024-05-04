#pragma once

namespace nsK2EngineLow {
	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		// ����������
		void Init(
			const char* tkmfilePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);

		// �X�P���g���̏�����
		void InitSkeleton(const char* filePath);

		// �A�j���[�V�����̏�����
		void InitAnimation(
			AnimationClip* animationClips,
			int numAnimationClips
		);

		// �A�j���[�V�����Đ�
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		// �A�j���[�V�����Đ���?
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// �A�j���[�V�����Đ��̑��x��ݒ�
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		//�A�j���[�V�����C�x���g
		//�����R�s�y���������ŁA������Ă�͒��ג�
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		// �X�V����
		void Update();

		//���W��ݒ�B
		void SetPosition(Vector3 pos)
		{
			m_pos = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}

		//��]��ݒ�
		void SetRotation(Quaternion rot)
		{
			m_rot = rot;
		}

		//�g�嗦��ݒ�B
		void SetScale(Vector3 scale)
		{
			m_scale = scale;
		}
		void SetScale(float x, float y, float z)
		{
			SetScale({ x, y, z });
		}

		Model& GetModel()
		{
			return m_model;
		}

		//�`�揈��
		void Draw(RenderContext& rc);

		//3D���f���̕`�揈��
		void OnRenderModel(RenderContext& rc) override;

	private:
		Model m_model;
		Skeleton		m_skeleton;
		AnimationClip* m_animationClips = nullptr;
		int				m_numAnimationClips = 0;
		Animation		m_animation;

		Vector3 m_pos = Vector3::Zero;
		Quaternion m_rot = Quaternion::Identity;
		Vector3 m_scale = Vector3::One;

		float m_animationSpeed = 1.0f;			//�A�j���[�V�����̍Đ��X�s�[�h
	};

}

