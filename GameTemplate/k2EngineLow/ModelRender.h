#pragma once

namespace nsK2EngineLow {
	class ModelRender : public Noncopyable
	{
	public:
		// 仮のライト、後でライトは別で管理できるようにしないといけない
		struct Light
		{
			Vector3 dirLigDirection;	// ディレクションライトの方向
			float pad0;					// パディング
			Vector3 dirColor;			// ディレクションライトのカラー
			float pad1;

			Vector3 ambientLight;		// アンビエントライト
			float pad2;

			Vector3 eyePos;				// 視点の位置
		};

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

		// 更新処理
		void Update();

		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion rot);
		void SetScale(Vector3 scale);

		// モデルのDrawを呼び出してるだけ
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

