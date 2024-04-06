#pragma once
class SpriteTest : public IGameObject
{
public:
	SpriteTest();
	~SpriteTest();

	void Update() override;
	void Render(RenderContext& rc) override;

private:
	SpriteRender m_spriteRender;

	Vector3 m_spritePos = Vector3::Zero;
	Quaternion m_spriteRot = Quaternion::Identity;
	Vector3 m_spriteScale = Vector3::One;

	ModelRender m_bgRender;
};


