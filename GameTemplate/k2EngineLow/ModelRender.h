#pragma once

namespace nsK2EngineLow {
	class ModelRender : public Noncopyable
	{
	public:
		// ���̃��C�g�A��Ń��C�g�͕ʂŊǗ��ł���悤�ɂ��Ȃ��Ƃ����Ȃ�
		struct Light
		{
			Vector3 dirLigDirection;	// �f�B���N�V�������C�g�̕���
			float pad0;					// �p�f�B���O
			Vector3 dirColor;			// �f�B���N�V�������C�g�̃J���[
			float pad1;

			Vector3 ambientLight;		// �A���r�G���g���C�g
			float pad2;

			Vector3 eyePos;				// ���_�̈ʒu
		};

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

		// ���f����Draw���Ăяo���Ă邾��
		void Draw(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

	private:
		Light m_light;
		Model m_model;
		Skeleton		m_skeleton;
		AnimationClip* m_animationClips = nullptr;
		int				m_numAnimationClips = 0;
		Animation		m_animation;

		Vector3 m_pos = Vector3::Zero;
		Quaternion m_rot = Quaternion::Identity;
		Vector3 m_scale = Vector3::One;
	};

}

