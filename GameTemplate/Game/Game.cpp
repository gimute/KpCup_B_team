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

	m_preSpriteRender.Init("Assets/sprite/mizuiro.DDS",1920,1080);
	m_preSpriteRender.SetMulColor(Vector4(0.7f, 0.7f, 0.7f, 1.0f));

	//HP���s���`�Ȏ��̉�ʃG�t�F�N�g�摜�̐ݒ�
	SpriteInitData initData;
	//DDS�t�@�C���i�摜�f�[�^�j�̃t�@�C���p�X���w�肷��
	//HP���s���`�Ȏ��̉�ʃG�t�F�N�g�̉摜�f�[�^���w�肷��
	initData.m_ddsFilePath[0] = "Assets/sprite/LowHpEffect.DDS";
	//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
	initData.m_fxFilePath = "Assets/shader/spritePinch.fx";
	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);
	//�X�v���C�g�̕��ƍ������w�肷��
	initData.m_width = static_cast<UINT>(1920);
	initData.m_height = static_cast<UINT>(1080);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;
	//
	m_pncSpriteRender.Init(initData);
	m_pncSpriteRender.SetPosition(Vector3{ 0.0f,0.0f,0.0f });
	m_pncSpriteRender.Update();
	

	//m_load->enState_FadeOut;
		//�Q�[���J�n�����[�h��ʕ\��
	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();

	//�w�i�̃I�u�W�F�N�g�����B
	m_background = NewGO<BackGround>(0, "background");
	
	m_levelRender.Init("Assets/levelData/map2level.tkl", [&](LevelObjectData_Render& objData)
	{
		if (objData.ForwardMatchName(L"player") == true)
		{
			//�v���C���[�̃I�u�W�F�N�g�����B
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

	//�Q�[���J�����̃I�u�W�F�N�g�����B
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	test = NewGO<EventCamera>(0,"camera");

	//HPUI�����
	m_hpui = NewGO<HpUi>(1, "UI");
	//�c�e�Ǘ��pUI�����
	m_remainingBulletsUi = NewGO<RemainingBulletsUi>(0, "UI");
	//�댯�M���\��Ui
	m_signalRailUi = NewGO<SignalRailUi>(1, "signalUi");
	//�C���t�H���[�V�����e�X�g
	m_infoUi = NewGO<InformationUi>(1, "UI");
	//登録
	m_infoUi->InitInformationSprite("Sousa", "Assets/modelData/ui_information/Sousa.DDS",960.0f,540.0f);
	m_infoUi->InitInformationSprite("Mission", "Assets/modelData/ui_information/Mission.DDS", 960.0f, 540.0f);
	//mapuiテスト6
	m_mapUi = NewGO<MapUi>(1, "mapUi");
	//�Q�[������BGM��ǂݍ���
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/m_main.wav");
	//�Q�[������BGM���Đ�����
	m_gameBgm = NewGO<SoundSource>(1);
	m_gameBgm->Init(1);
	m_gameBgm->Play(true);
	//HP���s���`�̎���BGM��ǂݍ���
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/m_hpLow.wav");

	m_hpLowBgm = NewGO<SoundSource>(11);

	IsPlayerMove(true);

	//���b�Z�[�W�̔w�i����
	m_massageBackGround.Init("Assets/modelData/maintimer/moya.DDS", 700.0f, 100.0f);
	m_massageBackGround.SetPosition(Vector3{ 0.0f,-370.0f,0.0f });
	m_massageBackGround.SetMulColor({ 0.0f,0.0f,0.0f,0.4f });
	m_massageBackGround.Update();

	//�h�A���J�������Ƃ�m�点�郁�b�Z�[�W����
	m_doorOpenMassage.SetText(L"�ǂ����̔����J�����悤��");
	m_doorOpenMassage.SetScale(0.5f);
	m_doorOpenMassage.SetPosition(Vector3(-350.0f, -420.0f, 0.0f));
	m_doorOpenMassage.SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

Game::~Game()
{
	if (m_gameState == enGameClear)
	{
		m_gametimer->m_game = nullptr;
	}
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
	if (!m_load->IsFade()&& !m_isFirstInfo)
	{
		m_infoUi->InitGOInformation("Sousa");
		m_isFirstInfo = true;
	}

	if (m_isFirstInfo && !m_isSecondInfo && !m_infoUi->IsInformationOpen())
	{
		m_infoUi->InitGOInformation("Mission");
		m_isSecondInfo = true;
	}

	if (m_isSecondInfo && !m_isTimerStart && !m_infoUi->IsInformationOpen())
	{
		//�Q�[���^�C�}�[�\��
		m_gametimer = NewGO<GameTimer>(1, "gametimer");
		m_isTimerStart = true;
		g_gameTime->IsSlowMotion(false);
	}

	if (g_pad[0]->IsTrigger(enButtonA) && !m_isTimerStart)
	{
		m_infoUi->InformationClose(false);
	}

	//�A���t�@�`�����l���̒���
	AlphaCalc();
	//m_pncSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_pncSpriteRender.Update();

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
		//�Q�[���N���A����UI��\��
		EventUiDelete(true);

		//m_hpLowBgm->Stop();

		if (!m_load->IsFade()) {
			//���g���폜����B
			DeleteGO(this);
			//�Q�[���N���A�̃I�u�W�F�N�g������B
			m_gameclear = NewGO<GameClear>(0, "gameclear");

		}
		break;

	case enGameOver:
		if (!m_load->IsFade()) {

			//�G�l�~�[�폜����
			for (auto& enemyhpui : m_EnemyHpUiList)
			{
				DeleteGO(enemyhpui);
			}

			for (auto& enemy : m_EnemyList)
			{
				DeleteGO(enemy);
			}
			//���g���폜����B
			DeleteGO(this);
			//�Q�[���I�[�o�[�̃I�u�W�F�N�g������B
			NewGO<GameOver>(0, "gameover");
		}
		break;
	}

	//�G�l�~�[�����ׂē|������
	if (m_EnemyQua == 0)
	{
		//�G�l�~�[�S�Ńt���O��ture�ɂ���
		m_enemyAllKillFlag = true;
	}

	//�v���C���[��HP��25�ȉ��Ȃ�
	if (m_hpui->GetNowHP() <= 25.0f)
	{
		//HP���s���`�Ȏ��̃G�t�F�N�g
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
		m_infoUi->InitGOInformation("Sousa");
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_infoUi->InitGOInformation("Mission");
	}

	//�G�l�~�[�����Ȃ����A�Q�[���X�e�[�g��Idle�Ȃ�
	if (!EnemyListExistence() and m_gameState == enIdle)
	{
		if (m_massageTimer <= 6.0f)
		{
			//�G�l�~�[�����ׂē|�����t���O��true��
			m_doorOpenMassageFlag = true;
			
			m_massageTimer += g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			m_doorOpenMassageFlag = false;
		}
		
	}
}

//�A���t�@�`�����l���̒���
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

//�������ԕ\��
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
	//�C�x���g�V�[�����Đ����Ȃ�
	if (test->IsEvent() == true)
	{
		m_gameState = enEvent;
	}

	//�Q�[���X�e�[�g��Idle�Ȃ�
	if (m_gameState == enIdle)
	{
		//�h�A���J���Ă���Ȃ�
		if (door1->GetDoorOpenFlag())
		{
			//�G�l�~�[�폜����
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

		//�v���C���[��HP��0�ȉ��Ȃ�
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

		//�C�x���g�V�[�����I��������
		if (test->IsEvent() == false)
		{
			switch (m_EventAfterState)
			{
			case enGameClear:
				//�t�F�[�h�A�E�g���J�n
				m_load->StartFadeOut();
				//�X�e�[�g���Q�[���N���A��
				m_gameState = enGameClear;

				break;

			case enGameOver:
				//�t�F�[�h�A�E�g���J�n
				m_load->StartFadeOut();
				//�X�e�[�g���Q�[���N���A��
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
		m_doorOpenMassage.Draw(rc);
	}
}