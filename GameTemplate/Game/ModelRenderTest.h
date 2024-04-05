#pragma once


//ModelRenderをためす為に用意したクラス
class ModelRenderTest : public IGameObject
{
public:

	ModelRenderTest();
	~ModelRenderTest();

	void Update() override;
	void Render(RenderContext& rc) override;

private:
	ModelRender m_pModelRender;
	ModelRender m_bgModelRender;

	enum EnAnimationClip {		//アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];

	int m_animeState = 0;

	Vector3 m_modelPos = Vector3::Zero;
	Vector3 m_modelScale = Vector3 :: One;
	Quaternion m_modelRot = Quaternion::Identity;
};

