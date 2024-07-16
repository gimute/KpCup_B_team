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
#include "MapUi.h"
#include "RemainingBulletsUi.h"
#include "InformationUi.h"
///////////////////////////////

Game::Game()
{
	//背景用画像の設定
	//現在の水色一色の画像を使用中
	m_preSpriteRender.Init("Assets/sprite/mizuiro.DDS",1920,1080);
	m_preSpriteRender.SetMulColor(Vector4(0.7f, 0.7f, 0.7f, 1.0f));

	//HPがピンチなときの画面エフェクト画像の設定
	//少し特殊なシェーダーを使いたいのでSpriteInitDataをこちらで用意する
	SpriteInitData initData;
	//画像ファイルパスを設定
	initData.m_ddsFilePath[0] = "Assets/sprite/LowHpEffect.DDS";
	//シェーダーのファイルパスを設定
	initData.m_fxFilePath = "Assets/shader/spritePinch.fx";
	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);
	//スプライトの幅と高さを設定
	initData.m_width = static_cast<UINT>(1920);
	initData.m_height = static_cast<UINT>(1080);
	//アルファブレンディングを半透明合成に設定
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;
	//用意したInitDataでスプライトレンダーを初期化
	m_pncSpriteRender.Init(initData);
	m_pncSpriteRender.Update();

	//ゲーム開始時のロード画面表示
	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();

	//背景オブジェクトを作成
	m_background = NewGO<BackGround>(0, "background");
	
	//レベルを使ってプレイヤー、ドア、エネミーを配置
	m_levelRender.Init("Assets/levelData/map2level.tkl", [&](LevelObjectData_Render& objData)
	{
		if (objData.ForwardMatchName(L"player") == true)
		{
			//プレイヤーのポインタは後で使うのでメンバ変数で受け取る
			m_player = NewGO<Player>(0, "player");
			m_player->m_position = objData.position;
			return true;
		}
		else if (objData.ForwardMatchName(L"door") == true)
		{
			//ドアのポインタは後で使うのでメンバ変数で受け取る
			door1 = NewGO<Door>(0, "door");
			door1->m_DoorMainPos = objData.position;
			door1->SetRotation(objData.rotation);
			return true;
		}
		else if (objData.ForwardMatchName(L"enemy") == true)
		{
			//エネミーのポインタは
			//EnemyクラスのStart関数でGameのm_EnemyListに登録するので
			//ローカル変数で受け取る
			Enemy* m_enemy = NewGO<Enemy>(0, "enemy");
			m_enemy->m_position = objData.position;

			return true;
		}
		return true;
	});

	//ゲームカメラのオブジェクトを作成
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	//イベントカメラクラスのオブジェクトを作成
	m_eventCamera = NewGO<EventCamera>(0,"camera");

	//HPUIを作成
	m_hpui = NewGO<HpUi>(1, "UI");
	//リロードUIを作成
	m_remainingBulletsUi = NewGO<RemainingBulletsUi>(0, "UI");
	//危険信号UIを作成
	m_signalRailUi = NewGO<SignalRailUi>(1, "signalUi");
	//マップUIを作成
	m_mapUi = NewGO<MapUi>(1, "mapUi");

	//インフォメーションを作成
	m_infoUi = NewGO<InformationUi>(1, "UI");
	//ゲーム開始時に表示するインフォメーション画像を設定
	m_infoUi->InitInformationSprite("Sousa", "Assets/modelData/ui_information/Sousa.DDS",960.0f,540.0f);
	m_infoUi->InitInformationSprite("Mission", "Assets/modelData/ui_information/Mission.DDS", 960.0f, 540.0f);
	//インフォメーション中はプレイヤーが動かないように設定
	IsPlayerMove(true);

	//ゲーム中のBGMを登録する
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/m_main.wav");
	//ゲーム中BGMを再生
	m_gameBgm = NewGO<SoundSource>(1);
	m_gameBgm->Init(1);
	m_gameBgm->Play(true);
	//ピンチ時に再生する心臓の音を登録
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/m_hpLow.wav");
	//心臓の音再生準備
	m_hpLowBgm = NewGO<SoundSource>(11);
	m_hpLowBgm->Init(11);

	//メッセージの背景
	m_massageBackGround.Init("Assets/modelData/maintimer/moya.DDS", 700.0f, 100.0f);
	m_massageBackGround.SetPosition(Vector3{ 0.0f,-370.0f,0.0f });
	m_massageBackGround.SetMulColor({ 0.0f,0.0f,0.0f,0.4f });
	m_massageBackGround.Update();

	//ドアが開いたときに表示するメッセージ
	m_doorOpenMassage.SetText(L"どこかの扉が開いたようだ");
	m_doorOpenMassage.SetScale(0.35f);
	m_doorOpenMassage.SetPosition(Vector3(-350.0f, -420.0f, 0.0f));
	m_doorOpenMassage.SetColor({ 1.0f,1.0f,1.0f,1.0f });

	//メッセージの最後に表示する点滅アンダーバー
	SpriteInitData UnKnDown;
	//画像ファイルパスを設定
	UnKnDown.m_ddsFilePath[0] = "Assets/modelData/maintimer/underBar.DDS";
	//シェーダーファイルパスを設定
	UnKnDown.m_fxFilePath = "Assets/shader/spriteUnKnDown.fx";
	UnKnDown.m_expandConstantBuffer = &m_alpha;
	UnKnDown.m_expandConstantBufferSize += sizeof(float);
	//スプライトの幅と高さを設定
	UnKnDown.m_width = static_cast<UINT>(25);
	UnKnDown.m_height = static_cast<UINT>(1.5);
	//アルファブレンディングを半透明合成に設定
	UnKnDown.m_alphaBlendMode = AlphaBlendMode_Trans;
	//用意したInitDataでスプライトレンダーを初期化
	m_spRenUnKnDown.Init(UnKnDown);
	m_spRenUnKnDown.SetPosition(Vector3{ 25.0f,-378.0f,0.0f });
	m_spRenUnKnDown.Update();
}

Game::~Game()
{
	//オブジェクト消去処理
	DeleteGO(m_infoUi);
	DeleteGO(m_remainingBulletsUi);
	DeleteGO(m_mapUi);
	DeleteGO(m_background);	
	DeleteGO(m_gamecamera);
	DeleteGO(m_hpui);
	DeleteGO(m_signalRailUi);
	DeleteGO(door1);
	DeleteGO(m_gameBgm);
	DeleteGO(m_hpLowBgm);
	DeleteGO(m_player);
	DeleteGO(m_gametimer);
}

bool Game::Start()
{
	//EnemyCamPosの初期化
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
	

	m_gameBgm->SetVolume(1.0f);
}

void Game::Update()
{
	//ゲームスタート時のインフォメーション処理
	GameStarInformation();

	//アルファチャンネルの調整
	AlphaCalc();
	m_pncSpriteRender.Update();
	m_spRenUnKnDown.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

	GameStateTransition();

	switch (m_gameState)
	{
	case enIdle:
		if (m_isTimerStart)
		{
			DisplayTime();
		}
		break;

	case enGameClear:
		//ゲームクリア中はUI非表示
		EventUiDelete(true);

		if (!m_load->IsFade()) {
			//自信を削除する
			DeleteGO(this);
			//ゲームクリアのオブジェクトを作る
			m_gameclear = NewGO<GameClear>(0, "gameclear");
			m_gameclear->SetClearTime(m_gametimer->m_minute, m_gametimer->m_second);
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
			//自信を削除する
			DeleteGO(this);
			//ゲームオーバーのオブジェクトを作る
			NewGO<GameOver>(0, "gameover");
		}
		break;
	}

	//エネミーが一人もいなければ
	if (m_EnemyQua == 0)
	{
		//エネミー全滅フラグを立てる
		m_enemyAllKillFlag = true;
	}

	//プレイヤーのHPが25以下なら
	if (m_hpui->GetNowHP() <= 25.0f)
	{
		//ピンチ時のエフェクト表示フラグを立てる
		m_pncDraw = true;

		//心臓の音を流す
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


	//エネミーが一人も居ない、
	//かつ、
	//ゲームステートがIdleなら
	if (!EnemyListExistence() and m_gameState == enIdle)
	{
		if (m_massageTimer <= 6.0f)
		{
			//ドアが開いたメッセージの表示フラグを立てる
			m_doorOpenMassageFlag = true;

			m_mapUi->InitGOMapUiPlayerPoint(door1->m_DoorMainPos);
			
			m_massageTimer += g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			m_doorOpenMassageFlag = false;
		}
		
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
	if (m_eventCamera->IsEvent() == true)
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

			m_eventCamera->StartScene(EventCamera::en_Scene_GameClear);

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
		if (m_eventCamera->IsEvent() == false)
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
				//フェードアウト開始
				m_load->StartFadeOut();
				//ステートをゲームオーバーに
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

void Game::SlowStart(float slowTime, int SlowStrength)
{
	g_gameTime->IsSlowMotion(true,SlowStrength);
	m_slowTime = slowTime;
}

void Game::GameStarInformation()
{
	switch (m_startInformationState)
	{
	case Game::enMission:
		if (!m_load->IsFade())
		{
			if (!m_infoUi->IsInformationOpen())
			{
				//ミッションのインフォメーションを出す
				m_infoUi->InitGOInformation("Mission");
			}
			else if (m_infoUi->IsInformationFullOpen() and g_pad[0]->IsTrigger(enButtonA))
			{
				m_infoUi->InformationClose(false);
				m_startInformationState = enSousa;
			}
		}
		break;

	case Game::enSousa:
		if (!m_infoUi->IsInformationOpen())
		{
			//操作のインフォメーションを出す
			m_infoUi->InitGOInformation("Sousa");
		}
		else if (m_infoUi->IsInformationFullOpen() and g_pad[0]->IsTrigger(enButtonA))
		{
			m_infoUi->InformationClose(false);
			m_startInformationState = enGameStart;
		}
		break;

	case Game::enGameStart:
		if (!m_infoUi->IsInformationOpen())
		{
			//タイマーをスタートする
			m_gametimer = NewGO<GameTimer>(1, "gametimer");
			m_isTimerStart = true;
			g_gameTime->IsSlowMotion(false);

			m_startInformationState = enEnd;
		}

	case Game::enEnd:

		break;

	default:
		break;
	}
}

void Game::Render(RenderContext& rc)
{
	m_preSpriteRender.Draw(rc);

	if (m_pncDraw == true)
	{
		m_pncSpriteRender.Draw(rc);
	}
	
	if (m_doorOpenMassageFlag)
	{
		m_massageBackGround.Draw(rc);
		m_spRenUnKnDown.Draw(rc);
		m_doorOpenMassage.Draw(rc);
	}
}