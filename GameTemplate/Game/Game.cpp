#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	m_modelrender.Init("Assets/modelData/background/proto_map/proto_map.tkm");
}

Game::~Game()
{

}

void Game::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}