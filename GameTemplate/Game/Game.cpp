#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Car.h"
#include "BackGround.h"

Game::Game()
{
	//�X�e�[�W�̃I�u�W�F�N�g�����B
	m_background = NewGO<BackGround>(0, "background");
	
	//�v���C���[�̃I�u�W�F�N�g�����B
	m_player = NewGO<Player>(0, "player");
	m_player->m_position = { 0.0f,0.0f,0.0f };

	//�Ԃ̃I�u�W�F�N�g�����B
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
	//m_modelrenderA.Draw(rc);
	//m_modelrenderB.Draw(rc);
	//m_modelrenderC.Draw(rc);
}