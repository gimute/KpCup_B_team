#pragma once

class Player;
class Car;
class BackGround;
class MiniTimer;

class Game : public IGameObject
{
public:
	Game();
	~Game();

	void Update();
	void Render(RenderContext& rc);

	Player* m_player;
	Car* m_car;
	BackGround* m_background;

	MiniTimer* m_minitimer = nullptr;

};

