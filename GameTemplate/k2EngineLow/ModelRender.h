#pragma once

#include "SceneLight.h"

namespace nsK2EngineLow {
	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		struct tsts
		{
		public:
			Light m_light;
			Matrix m_mt;
		};

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="tkmfilePath">モデルのファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="dithering">ディザリングフラグ、tureならディザリングを行う</param>
		/// <param name="isRecieverShadow">影を落とすか決めるフラグ、tureなら影が落ちる</param>
		void Init(
			const char* tkmfilePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool dithering = false,
			bool isRecieveShadow = false
		);

		void InitNormalModel(
			const char* tkmfilePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool dithering = false,
			bool isRecieveShadow = false
		);

		void InitShadowModel(
			const char* tkmFilePath, 
			EnModelUpAxis modelUpAxis
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

		//シャドウマップへの描画パスから呼ばれる処理
		void OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix) override;

		void SetShadowChasterFlag(bool flag)
		{
			m_isShadowChaster = flag;
		}

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


		//シャドウマップ関連

		Model m_shadowModel;

		tsts m_tsts;

		bool m_isShadowChaster = true;		//影を落とすか管理するフラグ
	};

}

