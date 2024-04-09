#pragma once
class MiniTimer;
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
	MiniTimer* m_minitimer = nullptr;
};

