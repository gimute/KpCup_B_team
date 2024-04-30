#pragma once
///////////////////////////////
class Player;
class Car;
class BackGround;
class GameCamera;
class MiniTimer;
class SpeedMeter;
class MainTimer;
class PointYazirushi;
class HpUi;
class Enemy;
///////////////////////////////
class Game : public IGameObject
{
public:
	Game();
	~Game();

	
	void Update();
	void Render(RenderContext& rc);
	void Delete_EnemyVec(const int num);
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
	PointYazirushi* m_pointyazi = nullptr;
	HpUi* m_hpui = nullptr;
	Enemy* m_enemy[3];
	std::vector<Enemy*> m_EnemyList;
	int m_EnemyQua = 0;
};

