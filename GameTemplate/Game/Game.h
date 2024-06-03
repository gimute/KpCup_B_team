#pragma once

#include "EnemyAttackPoint.h"
#include "EnemyCamPos.h"
#include "Bullet.h"
#include "Enemy.h"

//エネミーアタックポイントの数
//#define ENEMY_ATTACK_POINT_NUM 15

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
//
class Game : public IGameObject
{
public:
	enum EnGameState {
		enIdle,
		enGameClear,
		enGameOver
	};

	//エネミーの攻撃可能ポイント
	//このポイントを確保できているエネミーのみ攻撃可能にする
	//struct EnemyAttackPoint
	//{
	//	Vector3 m_position;			//アタックポイントの座標
	//	int m_number;				//アタックポイントの要素番号
	//	bool m_use = false;			//使用中か
	//};

	Game();
	~Game();

	void Update();
	//制限時間
	void DisplayTime();
	//ゲームクリアを通知する
	void NotifyGameClear();
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

	//メンバ変数
	Player* m_player;
	Enemy* m_enemy;
	BackGround* m_background;
	GameCamera* m_gamecamera;
	GameClear* m_gameclear;
	GameTimer* m_gametimer;
	SpriteRender m_spriterender; //スプライトレンダー。
	FontRender m_fontrender;	//フォントレンダー。
	HpUi* m_hpui = nullptr;
	Load* m_load = nullptr;
	//Load* m_load2 = nullptr;
	Door* door1;
	SignalRailUi* m_signalRailUi = nullptr;
	std::vector<Enemy*> m_EnemyList;
	std::vector<EnemyHpUi*> m_EnemyHpUiList;
	int m_EnemyQua = 0;
	EventCamera* test = nullptr;
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
	EnGameState m_gameState = enIdle;

	bool m_enemyAllKillFlag = false;	//敵を全滅させたか
};

