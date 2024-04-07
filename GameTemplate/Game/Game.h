#pragma once
class Game : public IGameObject
{
public:
	Game();
	~Game();

	void Update();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelrenderA;
	ModelRender m_modelrenderB;
	ModelRender m_modelrenderC;
};

