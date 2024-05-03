#pragma once
#include "Game.h"
class Game;
class Player;
class EnemyHpUi;

class Enemy : public IGameObject
{
public:
	//行動ステート
	enum EnEnemyState {
		enEnemyState_Idle,			//待機
		enEnemyState_Chase,			//追跡
		enEnemyState_Attack,		//攻撃
	};
	//アニメーション類/////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_Chase,			//追跡
		enAnimationClip_Attack,			//攻撃
		enAnimationClip_ShotStandby,    //攻撃待機
		enAnimationClip_Num,			//アニメーション数
	};
	void PlayAnimation();
	//ステート遷移関数///////////////////////////////////////////
	void ManageState();							//遷移処理
	void ProcessCommonStateTransition();		//共通遷移処理
	void ProcessIdleStateTransition();			//待機遷移
	void ProcessChaseStateTransition();			//追跡遷移
	void ProcessAttackStateTransition();		//攻撃遷移
	/////////////////////////////////////////////////////////////
	Enemy() {};
	~Enemy();
	bool Start();						//アップデート
	void Update();                                         //アップデート
	//モデルの回転をする関数
	void Rotation();                                       //回転
	void Collision();										//本体の当たり判定
	void Chase();										   //追跡
	void Attack();										   //攻撃

	//プレイヤーが視界内に居るか確かめる関数
	const bool SearchPlayer() const;

	//プレイヤーが一定範囲内に居るか確かめる関数
	const bool SearchChaseDistance() const;

	//プレイヤーが攻撃範囲内に居るか確かめる関数
	const bool SearchAttackDistance() const;

	//アタックポイントを確保できているか確認する関数
	//確保で来ていたらtrue、できていなかったらfalse;
	const bool HaveAttackPoint() const;

	void GetAttackPoint();

	//確保しているアタックポイントをリリースする関数
	void ReleaseAttackPoint();

	//モデルの描画処理
	void Render(RenderContext& rc);

	

	///////////////////////////////////////////////////////////
	//初期設定系統
	//メンバ関数宣言
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	Game::EnemyAttackPoint* m_enemeAttackPoint= nullptr;		//エネミーアタックポイント構造体のポインタ

	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	Vector3 m_movespeed;									//移動速度
	Vector3 m_position;										//座標
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	ModelRender m_modelRender;	//モデルレンダー
	CharacterController m_charaCon;        //キャラコン
	CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	int m_hp = 50;                           //HP
	//int m_sh = 0;							//シールド
	int m_Vectornum = 0;					//配列のナンバー
	float m_idleTimer= 0.0f;								//待機時間タイマー。
	float m_chaseTimer = 0.0f;								//追跡時間タイマー。
	float m_attackTimer = 5.0f;								//攻撃時間タイマー。
	float attacktime = 5.0f;
	float m_attackGotimer = 0.0;							//攻撃時間タイマー
	float attackGotime = 2.0;								//攻撃時間
	float m_attackcooltimer = 0.0f;
	float attackcooltime = 3.0f;
	float m_looptimer = 0.0f;								//ループタイマー
	float looptime = 1.16;									//ループ時間
	float m_mutekitimer = 0.0f;								//無敵タイマー
	float mutekitime = 0.1f;								//無敵時間
};

