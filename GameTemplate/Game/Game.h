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
	bool Start();

	void Update();
	//
	void AlphaCalc();
	//制限時間
	void DisplayTime();
	//ゲームクリアを通知する
	void NotifyGameClear();
	void NotifyGameOver();
	void Render(RenderContext& rc);
	void Delete_EnemyVec(const int num);

	/// <summary>
	/// エネミーの配列から配列番号を入れてその位置を取得
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	Vector3 GetEnemyListPos(int num);

	bool EnemyListExistence();

	bool EnemyListExistence(int num);

	void EventUiDelete(bool mode);

	void SetEnemyAttackState(const int Listnum, const Enemy::EnEnemyAttackSpeed& enemystate);

	//ゲームのステートを遷移する処理
	void GameStateTransition();
	//他のクラスから呼ぶためのゲームクリア処理。
	void GameClearProcess();
	//スローモージョンスタート
	void SlowStart(float SlowTime);

	bool ExistsEnemyListPtr(Enemy* enemy_ptr);
	//エネミーを全員倒したかのフラグを返す
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

	//メンバ変数
	Player* m_player;
	Enemy* m_enemy;
	BackGround* m_background;
	GameCamera* m_gamecamera;
	GameClear* m_gameclear;
	GameTimer* m_gametimer;
	SpriteRender m_spriterender; //スプライトレンダー。
	HpUi* m_hpui = nullptr;
	Load* m_load = nullptr;
	//Load* m_load2 = nullptr;
	Door* door1;
	SignalRailUi* m_signalRailUi = nullptr;
	RemainingBulletsUi* m_remainingBulletsUi = nullptr;
	InformationUi* m_infoUi;
	
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

private:
	EnemyAttackPoint m_enemyAttackPoint;
	EnemyCamPos m_enemyCamPos;
	LevelRender m_levelRender;

	PreSpriteRender m_preSpriteRender;
	SpriteRender m_pncSpriteRender;
	EnGameState m_gameState = enIdle;
	EnGameState m_EventAfterState;	//イベントシーン終了後に移行するステート

	bool m_enemyAllKillFlag = false;	//敵を全滅させたか
	bool m_alphaCalcBool;
	bool m_pncDraw = false;				//HPピンチ時エフェクト表示するか
	bool m_hpEffect = false;			//HPピンチ時エフェクトが現在、表示されているか
	bool m_hpLowBgmBool = false;		//HPピンチ時のBGMが流れているか
	float m_alpha = 0.1f;				//アルファチャンネルの調整用変数
	float m_slowTime = 0.0f;			//スローモーション時間

	SoundSource* m_gameBgm;
	SoundSource* m_hpLowBgm;
};

