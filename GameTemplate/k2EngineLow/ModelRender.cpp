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
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData initData;

		initData.m_tkmFilePath = tkmfilePath;
		initData.m_fxFilePath = "Assets/shader/model.fx";

		initData.m_expandConstantBuffer = g_sceneLight;
		initData.m_expandConstantBufferSize = sizeof(Light);

		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
		initData.m_vsEntryPointFunc = "VSMain";
		//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";

		InitSkeleton(tkmfilePath);
		if (m_skeleton.IsInited())
		{
			initData.m_skeleton = &m_skeleton;
		}

		InitAnimation(animationClips, numAnimationClips);

		initData.m_modelUpAxis = enModelUpAxis;

		//作成した初期化データをもとにモデルを初期化する、
		m_model.Init(initData);
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

		//スケルトンが初期化済みの場合、スケルトンの更新
		if (m_skeleton.IsInited())
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());
	}
}
