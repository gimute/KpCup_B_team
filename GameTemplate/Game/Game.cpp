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
#include "GameOver.h"
#include "GameClear.h"
#include "GameTimer.h"
#include "Load.h"
#include "Title.h"
#include "SignalRailUi.h"
#include "Bullet.h"
#include "Door.h"
///////////////////////////////

Game::Game()
{
	//ゲーム開始時ロード画面表示
	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();
	//m_load->enState_FadeOut;

	//背景のオブジェクトを作る。
	m_background = NewGO<BackGround>(0, "background");
	
	//ゲームタイマー表示
	m_gametimer = NewGO<GameTimer>(0, "gametimer");

	//プレイヤーのオブジェクトを作る。
	m_player = NewGO<Player>(0, "player");
	m_player->m_position = { 0.0f,0.0f,0.0f };

	//ゲームカメラのオブジェクトを作る。
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	//HPUIを作る
	m_hpui = NewGO<HpUi>(0, "UI");
	//危険信号表示Ui
	m_signalRailUi = NewGO<SignalRailUi>(3, "signalUi");

	//追いかけてくる敵を作る
	Enemy* m_enemy1 = NewGO<Enemy>(0, "enemy");
	m_enemy1->m_position = { -1250.0f,0.0f,300.0f };

	Enemy* m_enemy2 = NewGO<Enemy>(0, "enemy");
	m_enemy2->m_position = { -800.0f,0.0f,800.0f };

	//Enemy* m_enemy3 = NewGO<Enemy>(0, "enemy");
	//m_enemy3->m_position = { -1200.0f,0.0f,1000.0f };

	//Enemy* m_enemy4 = NewGO<Enemy>(0, "enemy");
	//m_enemy4->m_position = { -1000.0f,0.0f,500.0f };

	//Enemy* m_enemy5 = NewGO<Enemy>(0, "enemy");
	//m_enemy5->m_position = { -900.0f,0.0f,200.0f };

	//Enemy* m_enemy6 = NewGO<Enemy>(0, "enemy");
	//m_enemy6->m_position = { -900.0f,0.0f,1300.0f };

	//Enemy* m_enemy7 = NewGO<Enemy>(0, "enemy");
	//m_enemy7->m_position = { -800.0f,0.0f,700.0f };

	//Enemy* m_enemy8 = NewGO<Enemy>(0, "enemy");
	//m_enemy8->m_position = { -800.0f,0.0f,600.0f };

	//Enemy* m_enemy9 = NewGO<Enemy>(0, "enemy");
	//m_enemy9->m_position = { -1100.0f,0.0f,600.0f };

	//Enemy* m_enemy10 = NewGO<Enemy>(0, "enemy");
	//m_enemy10->m_position = { -850.0f,0.0f,300.0f };

	door1 = NewGO<Door>(0, "door");
	door1->m_DoorMainPos = { 0.0f,0.0f,380.0f };
}

Game::~Game()
{
	DeleteGO(m_background);	
	DeleteGO(m_gametimer);
	DeleteGO(m_gamecamera);
	DeleteGO(m_hpui);
	DeleteGO(m_signalRailUi);

	DeleteGO(m_player);
}

void Game::NotifyGameClear()
{
	m_isWaitFadeout = true;
	m_load->StartFadeOut();
}

void Game::Update()
{

	switch (m_gameState)
	{
	case enIdle:
		//エネミーをすべて倒したら
		if (m_EnemyQua == 0)
		{

			for (auto& enemyhpui : m_EnemyHpUiList)
			{
				DeleteGO(enemyhpui);
			}

			for (auto& enemy : m_EnemyList)
			{
				DeleteGO(enemy);
			}

			m_load->StartFadeOut();

			m_gameState = enGameClear;
		}

		if (m_hpui->GetNowHP() <= 0.0f)
		{
			//エネミーより先にゲームが消えてしまうとエラーを吐くので
			//一時的にここにエネミーを消す処理を記述
			//あとで修正
			//DeleteGO(m_hpui);
			for (auto& enemyhpui : m_EnemyHpUiList)
			{
				DeleteGO(enemyhpui);
			}

			for (auto& enemy : m_EnemyList)
			{
				DeleteGO(enemy);
			}

			m_load->StartFadeOut();

			m_gameState = enGameOver;
		}
		break;

	case enGameClear:
			if (!m_load->IsFade()) {
				//自身を削除する。
				DeleteGO(this);
				//プレイヤーのHPのUIを削除
				//DeleteGO(m_hpui);
				//ゲームクリアのオブジェクトをつくる。
				m_gameclear = NewGO<GameClear>(0, "gameclear");
			}
		break;

	case enGameOver:
		if (!m_load->IsFade()) {
			//自身を削除する。
			DeleteGO(this);
			//プレイヤーのHPのUIを削除
			//DeleteGO(m_hpui);
			//ゲームオーバーのオブジェクトをつくる。
			NewGO<GameOver>(0, "gameover");
		}
		break;
	}
	

	m_enemyAttackPoint.Update(m_player->GetPosition());
	m_hpui->Update();

	if (m_EnemyQua == 0)
	{
		door1->m_DoorOpen = true;
	}
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
	if (m_EnemyList.empty())
	{
		Vector3 dummy = { 0.0f,0.0f,1.0f };
		dummy *= 1000.0f;
		return dummy;
	}

	if (num + 1 > m_EnemyList.size())
	{
		Vector3 dummy = { 0.0f,0.0f,1.0f };
		dummy *= 1000.0f;
		return dummy;
	}
	return m_EnemyList[num]->m_position;
}

bool Game::EnemyListExistence()
{
	if (m_EnemyList.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Game::EnemyListExistence(int num)
{
	if (num + 1 > m_EnemyList.size())
	{
		return false;
	}

	return true;
}

void Game::SetEnemyAttackState(const int Listnum, const Enemy::EnEnemyAttackSpeed& enemystate)
{
	if (Listnum + 1 > m_EnemyList.size()
		|| m_EnemyList.empty())
	{
		return;
	}

	m_EnemyList[Listnum]->m_enemyAttackSpeed = enemystate;
	return;
}

void Game::Render(RenderContext& rc)
{
	
}