#pragma once
class Game : public IGameObject
{
public:
	Game();
	~Game();
	void Render(RenderContext& rc);
private:
	ModelRender m_modelrender;
};

