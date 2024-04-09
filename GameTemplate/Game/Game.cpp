#include "stdafx.h"
#include "Game.h"
//ÉeÉXÉgÅ´
#include "MiniTimer.h"
namespace {
	const Vector3 m_timerpos = { 50.0,0.0,0.0 };
}
Game::Game()
{
	m_modelrenderA.Init("Assets/modelData/background/proto_map/proto_map.tkm");
	m_modelrenderB.Init("Assets/modelData/player/proto_player/proto_player.tkm");

	m_modelrenderC.Init("Assets/modelData/car/proto_car/proto_car.tkm");
	m_modelrenderC.SetPosition(30.0f, 0.0f, 300.0f);
	m_modelrenderC.Update();

	m_minitimer = NewGO<MiniTimer>(1,"timer");
	m_minitimer->SetPosition(m_timerpos);
	m_minitimer->SetTimer(10);
}

Game::~Game()
{

}


void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	m_modelrenderA.Draw(rc);
	m_modelrenderB.Draw(rc);
	m_modelrenderC.Draw(rc);
}