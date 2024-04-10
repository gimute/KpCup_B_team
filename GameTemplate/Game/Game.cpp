#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Car.h"
#include "BackGround.h"
#include "MiniTimer.h"

Game::Game()
{
	//ƒXƒe[ƒW‚ÌƒIƒuƒWƒFƒNƒg‚ðì‚éB
	m_background = NewGO<BackGround>(0, "background");
	
	//ƒvƒŒƒCƒ„[‚ÌƒIƒuƒWƒFƒNƒg‚ðì‚éB
	m_player = NewGO<Player>(0, "player");
	m_player->m_position = { 0.0f,0.0f,0.0f };

	//ŽÔ‚ÌƒIƒuƒWƒFƒNƒg‚ðì‚éB
	m_car = NewGO<Car>(0, "car");
	m_car->m_position = { 30.0f,0.0f,300.0f };
}

Game::~Game()
{

}


void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}