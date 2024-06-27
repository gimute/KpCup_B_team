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
		//スケルトンの初期化
		InitSkeleton(tkmfilePath);

		//アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips);

		//基本のモデル初期化
		InitNormalModel(tkmfilePath, animationClips, numAnimationClips, enModelUpAxis, dithering, isRecieveShadow);

		//シャドウマップ描画用モデルの初期化
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

			// カメラの位置を設定。これはライトの位置
			lightCamera.SetPosition(0, 2000, 0);

			// カメラの注視点を設定。これがライトが照らしている場所
			lightCamera.SetTarget(0, 0, 0);

			// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
			lightCamera.SetUp(1, 0, 0);

			// ライトビュープロジェクション行列を計算している
			lightCamera.Update();

			m_tsts.m_light = *g_sceneLight->GetLightData();
			m_tsts.m_mt = lightCamera.GetViewProjectionMatrix();

			initData.m_expandConstantBuffer = &m_tsts;
			initData.m_expandConstantBufferSize = sizeof(m_tsts);

			//シャドウマップを拡張SRVに設定する
			initData.m_expandShaderResoruceView[0] = &(g_renderingEngine->GetShadowMapRenderTarget()->GetRenderTargetTexture());
		}
		else
		{
			initData.m_expandConstantBuffer = g_sceneLight->GetLightData();
			initData.m_expandConstantBufferSize = sizeof(Light);
		}

		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
		initData.m_vsEntryPointFunc = "VSMain";
		//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//スケルトンが初期化されていたら
		if (m_skeleton.IsInited())
		{
			initData.m_skeleton = &m_skeleton;
		}

		initData.m_modelUpAxis = enModelUpAxis;

		//作成した初期化データをもとにモデルを初期化する、
		m_model.Init(initData);
	}

	void ModelRender::InitShadowModel(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
	{
		ModelInitData initData;
		initData.m_tkmFilePath = tkmFilePath;
		initData.m_modelUpAxis = modelUpAxis;

		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
		//initData.m_vsEntryPointFunc = "VSMain";
		//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
		//initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			initData.m_skeleton = &m_skeleton;
		}

		initData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";

		m_shadowModel.Init(initData);
	}

	//tkmファイルのパスの.tkmの部分を.tksにして読み込む事で
	//いちいちtkmファイルパスとtksファイルパス両方を入力する必要が無くなる
	//代わりにtkmファイルがあるファイルにtksファイルがないと読み込めない。
	void ModelRender::InitSkeleton(const char* tkmFilePath)
	{
		//スケルトンのデータを読み込み。
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
				numAnimationClips //これなんでm_numAnimationClipsじゃないんだろ
			);
		}
	}

	void ModelRender::Update()
	{
		//ワールド行列更新
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);

		m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_scale);

		//スケルトンが初期化済みの場合、スケルトンの更新
		if (m_skeleton.IsInited())
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//アニメーションを進める
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
