#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	m_modelrenderA.Init("Assets/modelData/background/proto_map/proto_map.tkm");
	m_modelrenderB.Init("Assets/modelData/player/proto_player/proto_player.tkm");
}

Game::~Game()
{

}

void Game::Render(RenderContext& rc)
{
	m_modelrenderA.Draw(rc);
	m_modelrenderB.Draw(rc);
}