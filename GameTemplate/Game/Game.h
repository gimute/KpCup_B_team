#pragma once

class Player;
class Car;
class BackGround;
class Game : public IGameObject
{
public:
	Game();
	~Game();

	void Update();
	void Render(RenderContext& rc);

//ƒƒ“ƒo•Ï”
	ModelRender m_modelrenderA;
	ModelRender m_modelrenderB;
	ModelRender m_modelrenderC;
	Player* m_player;
	Car* m_car;
	BackGround* m_background;
};

