#pragma once
///////////////////////////////
class Player;
class Car;
class BackGround;
class GameCamera;
class MiniTimer;
class SpeedMeter;
class MainTimer;
///////////////////////////////
class Game : public IGameObject
{
public:
	Game();
	~Game();

	
	void Update();
	void Render(RenderContext& rc);

//メンバ変数
	Player* m_player;
	Car* m_car1;
	Car* m_car2;
	Car* m_car3;
	Car* m_car4;
	Car* m_car5;
	BackGround* m_background;
	GameCamera* m_gamecamera;
	MiniTimer* m_minitimer = nullptr;
	MainTimer* m_maintimer = nullptr;
	SpeedMeter* m_speedmeter = nullptr;
	FontRender m_fontrender;	//フォントレンダー。
	
};

