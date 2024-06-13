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
#include "EventCamera.h"
#include "sound/SoundEngine.h"
///////////////////////////////

Game::Game()
{

	m_preSpriteRender.Init("Assets/sprite/mizuiro.DDS",1920,1080);
	m_preSpriteRender.SetMulColor(Vector4(0.7f, 0.7f, 0.7f, 1.0f));
	//ゲーム開始時ロード画面表示
	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();
	//m_load->enState_FadeOut;

	//背景のオブジェクトを作る。
	m_background = NewGO<BackGround>(0, "background");
	
	//ゲームタイマー表示
	m_gametimer = NewGO<GameTimer>(0, "gametimer");

	m_levelRender.Init("Assets/levelData/map2level.tkl", [&](LevelObjectData_Render& objData)
	{
		if (objData.ForwardMatchName(L"player") == true)
		{
			//プレイヤーのオブジェクトを作る。
			m_player = NewGO<Player>(0, "player");
			m_player->m_position = objData.position;
			return true;
		}
		else if (objData.ForwardMatchName(L"door") == true)
		{
			door1 = NewGO<Door>(0, "door");
			door1->m_DoorMainPos = objData.position;
			door1->SetRotation(objData.rotation);
			return true;
		}
		else if (objData.ForwardMatchName(L"enemy") == true)
		{
			Enemy* m_enemy = NewGO<Enemy>(0, "enemy");
			m_enemy->m_position = objData.position;

			return true;
		}
		return true;
	});

	//ゲームカメラのオブジェクトを作る。
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	test = NewGO<EventCamera>(0,"camera");

	//HPUIを作る
	m_hpui = NewGO<HpUi>(0, "UI");
	//危険信号表示Ui
	m_signalRailUi = NewGO<SignalRailUi>(0, "signalUi");
	//ゲーム中のBGMを読み込む
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/m_main.wav");
	//ゲーム中のBGMを再生する
	m_gameBgm = NewGO<SoundSource>(1);
	m_gameBgm->Init(1);
	m_gameBgm->Play(true);

}

Game::~Game()
{
	DeleteGO(m_background);	
	DeleteGO(m_gamecamera);
	DeleteGO(m_hpui);
	DeleteGO(m_signalRailUi);
	DeleteGO(door1);
	DeleteGO(m_gameBgm);
	DeleteGO(m_player);
}

void Game::NotifyGameClear()
{
	m_isWaitFadeout = true;
	m_load->StartFadeOut();
}

void Game::Update()
{
	DisplayTime();

	GameStateTransition();

	switch (m_gameState)
	{
	case enIdle:
		//現状特別な処理は無し
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

	//エネミーをすべて倒したら
	if (m_EnemyQua == 0)
	{
		//エネミー全滅フラグをtureにする
		m_enemyAllKillFlag = true;
	}

	

	
	

	m_enemyAttackPoint.Update(m_player->GetPosition());
	m_hpui->Update();

	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		test->StartScene(EventCamera::en_Scene1_Door);
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		test->StartScene(EventCamera::en_Scene3_MapUp2);
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		test->StartScene(EventCamera::en_Scene2_MapUp1);
	}
}

//制限時間表示
void Game::DisplayTime()
{
	m_gametimer = FindGO<GameTimer>("gametimer");
	wchar_t wcsbuf[256];
	//制限時間を表示
	swprintf_s(wcsbuf, 256, L"%02d:%02d", int(m_gametimer->m_timer), int(m_gametimer->m_minit));
	//表示するテキストを設定
	m_fontrender.SetText(wcsbuf);
	//フォントの位置を設定
	m_fontrender.SetPosition(Vector3(-900.0f, -400.0f, 0.0f));
	//フォントの大きさを設定
	m_fontrender.SetScale(2.0f);
	//フォントの色を設定
	m_fontrender.SetColor({ 1.0f,0.0f,0.0f,1.0f });
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

bool Game::ExistsEnemyListPtr(Enemy* enemy_ptr)
{
	for (int i = 0; i < m_EnemyList.size(); i++)
	{
		Enemy* enemyList_ptr = m_EnemyList[i];
		if (enemyList_ptr == enemy_ptr)
		{
			return true;
		}
	}

	return false;
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

void Game::GameStateTransition()
{
	//イベントシーンが再生中なら
	if (test->IsEvent() == true)
	{
		m_gameState = enEvent;
	}

	//ゲームステートがIdleなら
	if (m_gameState == enIdle)
	{
		//ドアが開いているなら
		if (door1->GetDoorOpenFlag())
		{
			//エネミー削除処理
			for (auto& enemyhpui : m_EnemyHpUiList)
			{
				DeleteGO(enemyhpui);
			}

			for (auto& enemy : m_EnemyList)
			{
				DeleteGO(enemy);
			}

			test->StartScene(EventCamera::en_Scene2_MapUp1);
			m_EventAfterState = enGameClear;
			m_TempDelPlayer = true;
			return;
		}

		//プレイヤーのHPが0以下なら
		if (m_hpui->GetNowHP() <= 0.0f)
		{
			//エネミー削除処理
			for (auto& enemyhpui : m_EnemyHpUiList)
			{
				DeleteGO(enemyhpui);
			}

			for (auto& enemy : m_EnemyList)
			{
				DeleteGO(enemy);
			}

			test->StartScene(EventCamera::en_Scene2_MapUp1);
			m_EventAfterState = enGameOver;

			return;
		}
	}
	
	if (m_gameState == enEvent)
	{
		//イベントシーンが終了したら
		if (test->IsEvent() == false)
		{
			switch (m_EventAfterState)
			{
			case enGameClear:
				//フェードアウトを開始
				m_load->StartFadeOut();
				//ステートをゲームクリアに
				m_gameState = enGameClear;

				break;

			case enGameOver:
				//フェードアウトを開始
				m_load->StartFadeOut();
				//ステートをゲームクリアに
				m_gameState = enGameOver;

				DeleteGO(m_gametimer);

				break;

			default:
				m_gameState = m_EventAfterState;
			}
		}
	}

}

void Game::GameClearProcess()
{
	m_gameState = enGameClear;
	m_load->StartFadeOut();
}

void Game::EventUiDelete(bool mode)
{
	if (mode)
	{
		m_TempDelHpUi = false;

		m_TempDelGameTimer = false;

		m_TempDelSignalUi = false;
	}
	else
	{
		m_TempDelHpUi = true;

		m_TempDelGameTimer = true;

		m_TempDelSignalUi = true;
	}
}

void Game::Render(RenderContext& rc)
{
	if (m_TempDelGameTimer == true)
	{
		m_fontrender.Draw(rc);
	}
	m_preSpriteRender.Draw(rc);
}