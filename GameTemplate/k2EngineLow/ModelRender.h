#pragma once

namespace nsK2EngineLow {
	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		// 初期化処理
		void Init(
			const char* tkmfilePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);

		// スケルトンの初期化
		void InitSkeleton(const char* filePath);

		// アニメーションの初期化
		void InitAnimation(
			AnimationClip* animationClips,
			int numAnimationClips
		);

		// アニメーション再生
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		// アニメーション再生中?
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// アニメーション再生の速度を設定
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		//アニメーションイベント
		//ただコピペしただけで、何やってるは調べ中
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		// 更新処理
		void Update();

		//座標を設定。
		void SetPosition(Vector3 pos)
		{
			m_pos = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}

		//回転を設定
		void SetRotation(Quaternion rot)
		{
			m_rot = rot;
		}

		//拡大率を設定。
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

		//描画処理
		void Draw(RenderContext& rc);

		//3Dモデルの描画処理
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

		float m_animationSpeed = 1.0f;			//アニメーションの再生スピード
	};

}

