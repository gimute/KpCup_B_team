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

	//スピードメータのUIを作る。
	m_speedmeter = NewGO<SpeedMeter>(1, "UI");

	//メインタイマーのUIを作る
	m_maintimer = NewGO<MainTimer>(2, "UI");
	m_maintimer->SetTimer(10);

	//HPUIを作る
	m_hpui = NewGO<HpUi>(3, "UI");

	//追いかけてくる敵を作る
	Enemy* m_enemy1 = NewGO<Enemy>(0, "enemy");
	m_enemy1->m_position = { 10.0f,0.0f,500.0f };

	Enemy* m_enemy2 = NewGO<Enemy>(0, "enemy");
	m_enemy2->m_position = { 200.0f,0.0f,500.0f };

	Enemy* m_enemy3 = NewGO<Enemy>(0, "enemy");
	m_enemy3->m_position = { 400.0f,0.0f,500.0f };

	Enemy* m_enemy4 = NewGO<Enemy>(0, "enemy");
	m_enemy4->m_position = { -200.0f,0.0f,500.0f };

	Enemy* m_enemy5 = NewGO<Enemy>(0, "enemy");
	m_enemy5->m_position = { -400.0f,0.0f,500.0f };

	Enemy* m_enemy6 = NewGO<Enemy>(0, "enemy");
	m_enemy6->m_position = { 10.0f,0.0f,-500.0f };

	Enemy* m_enemy7 = NewGO<Enemy>(0, "enemy");
	m_enemy7->m_position = { -200.0f,0.0f,-500.0f };

	Enemy* m_enemy8 = NewGO<Enemy>(0, "enemy");
	m_enemy8->m_position = { -400.0f,0.0f,-500.0f };

	Enemy* m_enemy9 = NewGO<Enemy>(0, "enemy");
	m_enemy9->m_position = { 200.0f,0.0f,-500.0f };

	Enemy* m_enemy10 = NewGO<Enemy>(0, "enemy");
	m_enemy10->m_position = { 400.0f,0.0f,-500.0f };

}

Game::~Game()
{

}


void Game::Update()
{
	EnemyAttackPointUpdate();

	if (g_pad[0]->IsTrigger(enButtonY))
	{
		m_EnemyHpUiList[0]->DecreaseHP(20);
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

//エネミーアタックポイント関連///////////////////////////////////////////////////
void Game::EnemyAttackPointUpdate()
{
	Vector3 pos = m_player->GetPosition();

	//エネミーアタックポイントをプレイヤーのポジションの周囲に等間隔で並べる
	for (int i = 0; i < ENEMY_ATTACK_POINT_NUM; i++)
	{
		//プレイヤーのポジションからポイントまでの距離
		float lenge = 200.0f;

		//ポイントを設置する方向
		Vector3 direction = g_vec3Front;

		//ポイントを設置する方向を回転させるクオータニオン
		Quaternion directionRot = g_quatIdentity;

		//360度をエネミーアタックポイント配列の要素数で割って等間隔で置くための角度を設定する
		directionRot.SetRotationDegY(360.0f / ENEMY_ATTACK_POINT_NUM);

		//ポイントの座標を設定する
		for (int i = 0; i < ENEMY_ATTACK_POINT_NUM; i++)
		{
			m_enemyAttackPointList[i].m_position = pos + direction * lenge;

			//設置する方向を回転
			directionRot.Apply(direction);
		}
	}
}

Game::EnemyAttackPoint* Game::GetEnemyAttackPoint()
{
	//使用中じゃないポイントを探す
	for (int i = 0; i < ENEMY_ATTACK_POINT_NUM; i++)
	{
		//未使用のポイントが見つかれば
		if (m_enemyAttackPointList[i].m_use == false)
		{
			//アドレスを返す
			m_enemyAttackPointList[i].m_use = true;
			return &m_enemyAttackPointList[i];
		}
	}

	//無ければnullptrを返す
	return nullptr;

}
/////////////////////////////////////////////////////////////////////////////////


void Game::Render(RenderContext& rc)
{
	
}