#include "stdafx.h"
#include "Game.h"
///////////////////////////////
#include "Player.h"
#include "Car.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "SpeedMeter.h"
#include "MiniTimer.h"
#include "MainTimer.h"
#include "PointYazirushi.h"
#include "HpUi.h"
#include "Enemy.h"
///////////////////////////////

Game::Game()
{
	//背景のオブジェクトを作る。
	m_background = NewGO<BackGround>(0, "background");
	
	//プレイヤーのオブジェクトを作る。
	m_player = NewGO<Player>(0, "player");
	m_player->m_position = { 0.0f,0.0f,0.0f };

	//車のオブジェクトを作る。
	//m_car1 = NewGO<Car>(0, "car");
	//m_car1->m_position = { 30.0f,0.0f,3000.0f };

	//m_car2 = NewGO<Car>(0, "car");
	//m_car2->m_position = { 1000.0f,0.0f,0.0f };

	//m_car3 = NewGO<Car>(0, "car");
	//m_car3->m_position = { -500.0f,0.0f,-2000.0f };

	//m_car4 = NewGO<Car>(0, "car");
	//m_car4->m_position = { 400.0f,0.0f,-500.0f };

	//m_car5 = NewGO<Car>(0, "car");
	//m_car5->m_position = { -1200.0f,0.0f,3000.0f };


	//ゲームカメラのオブジェクトを作る。
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	//スピードメータのUIを作る。
	m_speedmeter = NewGO<SpeedMeter>(1, "UI");

	//メインタイマーのUIを作る
	m_maintimer = NewGO<MainTimer>(2, "UI");
	m_maintimer->SetTimer(10);

	//HPUIを作る
	m_hpui = NewGO<HpUi>(3, "UI");

	//矢印を作る
	//m_pointyazi = NewGO<PointYazirushi>(0);
	//m_pointyazi->SetPosition(m_player->m_position);
	//m_pointyazi->SetTarget(m_car5->m_position);

	//追いかけてくる敵を作る
	m_enemy = NewGO<Enemy>(0, "enemy");
	m_enemy->m_position = { 10.0f,0.0f,500.0f };
}

Game::~Game()
{

}


void Game::Update()
{
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		m_hpui->DecreaseHP(20);
	}
}



void Game::Render(RenderContext& rc)
{
	
}