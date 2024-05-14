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
#include "EnemyHpUi.h"
#include "EnemyAttackPoint.h"
#include "EnemyCamPos.h"
///////////////////////////////

Game::Game()
{
	//背景のオブジェクトを作る。
	m_background = NewGO<BackGround>(0, "background");
	
	//プレイヤーのオブジェクトを作る。
	m_player = NewGO<Player>(0, "player");
	m_player->m_position = { 0.0f,0.0f,0.0f };

	//ゲームカメラのオブジェクトを作る。
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	//HPUIを作る
	m_hpui = NewGO<HpUi>(3, "UI");

	//追いかけてくる敵を作る
	Enemy* m_enemy1 = NewGO<Enemy>(0, "enemy");
	m_enemy1->m_position = { -1250.0f,0.0f,300.0f };

	Enemy* m_enemy2 = NewGO<Enemy>(0, "enemy");
	m_enemy2->m_position = { -800.0f,0.0f,800.0f };

	Enemy* m_enemy3 = NewGO<Enemy>(0, "enemy");
	m_enemy3->m_position = { -1200.0f,0.0f,1000.0f };

	Enemy* m_enemy4 = NewGO<Enemy>(0, "enemy");
	m_enemy4->m_position = { -1000.0f,0.0f,500.0f };

	Enemy* m_enemy5 = NewGO<Enemy>(0, "enemy");
	m_enemy5->m_position = { -900.0f,0.0f,200.0f };

	Enemy* m_enemy6 = NewGO<Enemy>(0, "enemy");
	m_enemy6->m_position = { -900.0f,0.0f,1300.0f };

	Enemy* m_enemy7 = NewGO<Enemy>(0, "enemy");
	m_enemy7->m_position = { -800.0f,0.0f,700.0f };

	Enemy* m_enemy8 = NewGO<Enemy>(0, "enemy");
	m_enemy8->m_position = { -800.0f,0.0f,600.0f };

	Enemy* m_enemy9 = NewGO<Enemy>(0, "enemy");
	m_enemy9->m_position = { -1100.0f,0.0f,600.0f };

	Enemy* m_enemy10 = NewGO<Enemy>(0, "enemy");
	m_enemy10->m_position = { -850.0f,0.0f,300.0f };

	/*for (int i = 0; i < ENEMY_ATTACK_POINT_NUM; i++)
	{
		m_enemyAttackPointList[i].m_number = i;
	}*/
}

Game::~Game()
{

}


void Game::Update()
{
	m_enemyAttackPoint.Update(m_player->GetPosition());

	m_hpui->Update();
  
	m_enemyCamPos.EnemyCamPosConfirmation();
}

void Game::Delete_EnemyVec(const int num)
{
	m_EnemyList.erase(m_EnemyList.begin() + num);
	m_EnemyHpUiList.erase(m_EnemyHpUiList.begin() + num);
	for (int VecNow = num; VecNow < m_EnemyList.size(); VecNow++) {
		m_EnemyList[VecNow]->m_Vectornum -= 1;
		m_EnemyHpUiList[VecNow]->m_Vectornum -= 1;
	}
	m_EnemyQua--;
}

Vector3 Game::GetEnemyListPos(int num)
{
	if (num + 1 > m_EnemyList.size())
	{
		Vector3 dummy = { 0.0f,0.0f,1.0f };
		dummy *= 1000.0f;
		return dummy;
	}
	return m_EnemyList[num]->m_position;
}


void Game::Render(RenderContext& rc)
{

}