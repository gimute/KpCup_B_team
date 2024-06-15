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
#include "EventModel.h"
#include "sound/SoundEngine.h"
///////////////////////////////

Game::Game()
{

	m_preSpriteRender.Init("Assets/sprite/mizuiro.DDS",1920,1080);
	m_preSpriteRender.SetMulColor(Vector4(0.7f, 0.7f, 0.7f, 1.0f));

	//HPがピンチな時の画面エフェクト画像
	SpriteInitData initData;
	//DDSファイル（画像データ）のファイルパスを指定する
	//HPがピンチな時の画面エフェクトの画像データを指定する
	initData.m_ddsFilePath[0] = "Assets/sprite/LowHpEffect.DDS";
	//Sprite表示用のシェーダーのファイルパスを指定する
	initData.m_fxFilePath = "Assets/shader/spritePinch.fx";
	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);
	//スプライトの幅と高さを指定する
	initData.m_width = static_cast<UINT>(1920);
	initData.m_height = static_cast<UINT>(1080);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;
	//
	m_pncSpriteRender.Init(initData);
	m_pncSpriteRender.SetPosition(Vector3{ 0.0f,0.0f,0.0f });
	m_pncSpriteRender.Update();
	
	//HPがピンチな時の画面エフェクトを読み込む
	//m_pncSpriteRender.Init("Assets/sprite/LowHpEffect.DDS",1920,1080);
	//m_pncSpriteRender.SetMulColor(Vector4())
	//ゲーム開始時ロード画面表示
	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();
	//m_load->enState_FadeOut;

	//背景のオブジェクトを作る。
	m_background = NewGO<BackGround>(0, "background");
	
	//ゲームタイマー表示
	m_gametimer = NewGO<GameTimer>(1, "gametimer");

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

	test2 = NewGO<EventModel>(0, "camera");

	//HPUIを作る
	m_hpui = NewGO<HpUi>(1, "UI");
	//危険信号表示Ui
	m_signalRailUi = NewGO<SignalRailUi>(1, "signalUi");
	//ゲーム中のBGMを読み込む
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/m_main.wav");
	//ゲーム中のBGMを再生する
	m_gameBgm = NewGO<SoundSource>(1);
	m_gameBgm->Init(1);
	m_gameBgm->Play(true);
	//HPがピンチの時のBGMを読み込む
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/m_hpLow.wav");

	m_hpLowBgm = NewGO<SoundSource>(11);
}

Game::~Game()
{
	if (m_gameState == enGameClear)
	{
		m_gametimer->m_game = nullptr;
	}
	
	DeleteGO(m_background);	
	DeleteGO(m_gamecamera);
	DeleteGO(m_hpui);
	DeleteGO(m_signalRailUi);
	DeleteGO(door1);
	DeleteGO(m_gameBgm);
	DeleteGO(m_hpLowBgm);
	DeleteGO(m_player);
}

bool Game::Start()
{
	m_enemyCamPos.Init();
	return true;
}

void Game::NotifyGameClear()
{
	m_isWaitFadeout = true;
	m_load->StartFadeOut();
}

void Game::NotifyGameOver()
{
	m_gameState = enGameOver;
	m_load->StartFadeOut();
	DeleteGO(m_gametimer);

	m_gameBgm->SetVolume(1.0f);
}

void Game::Update()
{
	//アルファチャンネルの調整
	AlphaCalc();
	//m_pncSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_pncSpriteRender.Update();

	

	GameStateTransition();

	switch (m_gameState)
	{
	case enIdle:
		DisplayTime();
		break;

	case enGameClear:
		//ゲームクリア中はUI非表示
		EventUiDelete(true);

		m_hpLowBgm->Stop();

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

			//エネミー削除処理
			for (auto& enemyhpui : m_EnemyHpUiList)
			{
				DeleteGO(enemyhpui);
			}

			for (auto& enemy : m_EnemyList)
			{
				DeleteGO(enemy);
			}
			//自身を削除する。
			DeleteGO(this);
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
	//プレイヤーのHPが25以下なら
	if (m_hpui->GetNowHP() <= 25.0f)
	{
		//HPがピンチな時のエフェクト
		m_pncDraw = true;

		if (m_hpLowBgm->IsPlaying() == false && m_gameState == enIdle && m_hpui->GetNowHP() > 0.0f)
		{
			m_hpLowBgm->Init(11);
			m_hpLowBgm->Play(true);
			m_hpLowBgm->SetVolume(3.0f);
			m_gameBgm->SetVolume(0.5f);
			m_hpLowBgmBool = true;
		}
	}
	else
	{
		m_pncDraw = false;
		m_gameBgm->SetVolume(1.0f);
	}
	

	if (m_slowTime > 0.0f)
	{
		m_slowTime -= g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		g_gameTime->IsSlowMotion(false);
	}

	m_enemyAttackPoint.Update(m_player->GetPosition());
	m_hpui->Update();
	//m_pncSpriteRender.Update();

	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		test->StartScene(EventCamera::en_Scene1_Door);
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		test->StartScene(EventCamera::en_Scene2_MapUp1);
	}
}

//アルファチャンネルの調整
void Game::AlphaCalc()
{
	if (m_hpEffect)
	{
		m_alpha = 0.0f;
	}
	if (m_alpha >= 0.6f) {
		m_alphaCalcBool = false;
	}
	if (m_alphaCalcBool)
	{
		m_alpha += 0.02f;
		return;
	}
	m_alpha -= 0.01f;
	if (m_alpha <= 0.1f)
	{
		m_alphaCalcBool = true;
	}
}

//制限時間表示
void Game::DisplayTime()
{
	m_gametimer->FontSet();
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

			m_hpEffect = true;
			m_gameBgm->SetVolume(1.0f);

			test->StartScene(EventCamera::en_Scene_GameClear);

			m_EventAfterState = enGameClear;
			m_player->SetEvent(Player::enGameClear);
			return;
		}

		//プレイヤーのHPが0以下なら
		if (m_hpui->GetNowHP() <= 0.0f)
		{
			m_hpLowBgm->Stop();
			return;
		}
		m_hpEffect = false;
	}
	
	if (m_gameState == enEvent)
	{
		if (m_hpLowBgm->IsPlaying())
		{
			m_hpLowBgm->Stop();
		}

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

void Game::SlowStart(float slowTime)
{
	g_gameTime->IsSlowMotion(true);
	m_slowTime = slowTime;
}

void Game::Render(RenderContext& rc)
{
	m_preSpriteRender.Draw(rc);

	if (m_pncDraw == true)
	{
		m_pncSpriteRender.Draw(rc);
	}
	
}