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
class SignalRailUi;
class Door;
//
class Game : public IGameObject
{
public:
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



	void SetEnemyAttackState(const int Listnum, const Enemy::EnEnemyAttackSpeed& enemystate);

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
	BackGround* m_background;
	GameCamera* m_gamecamera;
	FontRender m_fontrender;	//フォントレンダー。
	HpUi* m_hpui = nullptr;
	Door* door1;
	SignalRailUi* m_signalRailUi = nullptr;
	std::vector<Enemy*> m_EnemyList;
	std::vector<EnemyHpUi*> m_EnemyHpUiList;
	int m_EnemyQua = 0;
	

private:
	EnemyAttackPoint m_enemyAttackPoint;
	EnemyCamPos m_enemyCamPos;

	PreSpriteRender m_preSpriteRender;
};

