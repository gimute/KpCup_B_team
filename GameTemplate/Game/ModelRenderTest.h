#pragma once
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

	Vector3 m_modelPos = Vector3::Zero;
	Vector3 m_modelScale = Vector3 :: One;
	Quaternion m_modelRot = Quaternion::Identity;
};

