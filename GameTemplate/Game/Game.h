#pragma once

#include "EnemyAttackPoint.h"
#include "EnemyCamPos.h"
#include "Bullet.h"
#include "Enemy.h"
#include "sound/SoundSource.h"


///////////////////////////////
class Player;
class BackGround;
class GameCamera;
class MiniTimer;
class SpeedMeter;
class MainTimer;
class HpUi;
class Enemy;
class EnemyHpUi;
class GameClear;
class GameTimer;
class Load;
class Title;
class SignalRailUi;
class Door;
class EventCamera;
class MapUi;
class RemainingBulletsUi;
class InformationUi;
class CountDownUI;
//
class Game : public IGameObject
{
public:
	enum EnGameState {
		enIdle,
		enGameClear,
		enGameOver,
		enEvent
	};

	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/// <summary>
	/// �G�l�~�[�̔z�񂩂�z��ԍ������Ă��̈ʒu���擾
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	Vector3 GetEnemyListPos(int num);

public:
	//void TimerStart();
	//
	void AlphaCalc();
	//��������
	void DisplayTime();
	//�Q�[���N���A��ʒm����
	void NotifyGameClear();
	void NotifyGameOver();
	void Delete_EnemyVec(const int num);


	bool EnemyListExistence();

	bool EnemyListExistence(int num);

	void EventUiDelete(bool mode);

	void SetEnemyAttackState(const int Listnum, const Enemy::EnEnemyAttackSpeed& enemystate);

	//�Q�[���̃X�e�[�g��J�ڂ��鏈��
	void GameStateTransition();
	//���̃N���X����ĂԂ��߂̃Q�[���N���A�����B
	void GameClearProcess();
	//�X���[���[�W�����X�^�[�g
	void SlowStart(float SlowTime,int SlowStrength);

	bool ExistsEnemyListPtr(Enemy* enemy_ptr);
	//�G�l�~�[��S���|�������̃t���O��Ԃ�
	bool GetEnemyAllKillFlag()
	{
		return m_enemyAllKillFlag;
	}

	EnemyAttackPoint* GetEnemyAttackPointObject()

	{
		return &m_enemyAttackPoint;
	}

	EnemyCamPos* GetEnemyCamPosInstance()
	{
		return &m_enemyCamPos;
	}

	Enemy* GetEnemyListInstance(int Listnum)
	{
		return m_EnemyList[Listnum];
	}

	EnGameState GetGmaeState()
	{
		return m_gameState;
	}

	int GetEnemyListSize()
	{
		return m_EnemyList.size();
	}

	void IsPlayerMove(bool playermove)
	{
		m_isPlayerMove = playermove;
	}

	bool GetPlayerMove()
	{
		return m_isPlayerMove;
	}

	bool GetTimerStart()
	{
		return m_isTimerStart;
	}

	bool GetThirdCD()
	{
		return m_thirdCD;
	}

	bool GetSecondCD()
	{
		return m_secondCD;
	}

	bool GetFirstCD()
	{
		return m_startCD;
	}

	bool GetStartCD()
	{
		return m_startCD;
	}

public:
	//�C���X�^���X
	Player* m_player;
	Enemy* m_enemy;
	BackGround* m_background;
	GameCamera* m_gamecamera;
	GameClear* m_gameclear;
	GameTimer* m_gametimer;
	HpUi* m_hpui = nullptr;
	Load* m_load = nullptr;

	SpriteRender m_spriterender; //�X�v���C�g�����_�[�B
	
	//Load* m_load2 = nullptr;
	Door* door1;
	SignalRailUi* m_signalRailUi = nullptr;
	RemainingBulletsUi* m_remainingBulletsUi = nullptr;
	InformationUi* m_infoUi;
	CountDownUI* m_coDoUI = nullptr;
	
	std::vector<Enemy*> m_EnemyList;
	std::vector<EnemyHpUi*> m_EnemyHpUiList;
	int m_EnemyQua = 0;
	EventCamera* test = nullptr;
	MapUi* m_mapUi = nullptr;
	bool m_isSaveClearTime = false;
	bool m_isWaitFadeout = false;

	bool m_TempDelHpUi = true;
	bool m_TempDelSignalUi = true;
	bool m_TempDelGameTimer = true;
	bool m_TempDelPlayer = false;

	
	float m_minit = 0.0f;

private:
	EnemyAttackPoint m_enemyAttackPoint;
	EnemyCamPos m_enemyCamPos;
	LevelRender m_levelRender;

	PreSpriteRender m_preSpriteRender;
	SpriteRender m_pncSpriteRender;
	EnGameState m_gameState = enIdle;
	EnGameState m_EventAfterState;	//�C�x���g�V�[���I����Ɉڍs����X�e�[�g

	bool m_enemyAllKillFlag = false;	//�G��S�ł�������
	bool m_alphaCalcBool;
	bool m_pncDraw = false;				//HP�s���`���G�t�F�N�g�\�����邩
	bool m_hpEffect = false;			//HP�s���`���G�t�F�N�g�����݁A�\������Ă��邩
	bool m_hpLowBgmBool = false;		//HP�s���`����BGM������Ă��邩
	bool m_isPlayerMove = false;		//�v���C���[���ړ��ł��邩�ǂ���
	bool m_isTimerStart = false;		// �^�C�}�[���X�^�[�g���Ă��邩
	bool m_thirdCD = false;
	bool m_secondCD = false;
	bool m_firstCD = false;
	bool m_startCD = false;
	
	float m_alpha = 0.1f;				//�A���t�@�`�����l���̒����p�ϐ�
	float m_slowTime = 0.0f;			//�X���[���[�V��������

	SoundSource* m_gameBgm;
	SoundSource* m_hpLowBgm;

	SpriteRender m_massageBackGround;	//���b�Z�[�W�w�i
	SpriteRender m_spRenUnKnDown;		//���b�Z�[�W�̍Ō�ɃA���_�[�o�[
	FontRender m_doorOpenMassage;		//�h�A���J�������Ƃ�m�点�郁�b�Z�[�W
	bool m_doorOpenMassageFlag = false;	//�h�A���J�������Ƃ�m�点�郁�b�Z�[�W�\���t���O
	bool m_timerDraw = true;			//�^�C�}�[���`�悳��Ă��邩
	float m_massageTimer;				//���b�Z�[�W��\�����鎞�Ԃ𐧌䂷��^�C�}�[
};

