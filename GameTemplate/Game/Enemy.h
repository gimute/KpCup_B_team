#pragma once
#include "Game.h"
class Game;
class Player;
class EnemyHpUi;
class Bullet;

class Enemy : public IGameObject
{
public:
	//行動ステート
	enum EnEnemyState {
		enEnemyState_Idle,			//待機
		enEnemyState_Chase,			//追跡
		enEnemyState_Attack,		//攻撃
		enEnemyState_Stand,			//構え
		enEnemyState_ReceiveDamage,	//被ダメージ
		enEnemyState_Down			//ダウン
	};
	//アニメーション類/////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_Chase,			//追跡
		enAnimationClip_Attack,			//攻撃
		enAnimationClip_ShotStandby,    //攻撃待機
		enAnimationClip_Damage,			//被ダメージモーション
		enAnimationClip_Num,			//アニメーション数
	};
	void PlayAnimation();
	//ステート遷移関数///////////////////////////////////////////
	void ManageState();							//遷移処理
	void ProcessCommonStateTransition();		//共通遷移処理
	void ProcessIdleStateTransition();			//待機遷移
	void ProcessChaseStateTransition();			//追跡遷移
	void ProcessAttackStateTransition();		//攻撃遷移
	void ProcessStandStateTransition();			//構え遷移
	void ProcessReceiveDamageStateTransition();	//被ダメージ遷移
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

	//現在自分が所持しているアタックポイントを使用中にする
	void SetAttackPointIsUse()
	{
		m_enemyAttackPoint->m_use = true;
		m_useAttacPoint = true;
	}
	//現在自分が所持しているアタックポイントを未使用にする
	void SetAttackPointIsUnUse()
	{
		m_enemyAttackPoint->m_use = false;
		m_useAttacPoint = false;
	}

	//アニメーションイベント
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	//モデルの描画処理
	void Render(RenderContext& rc);

	

	///////////////////////////////////////////////////////////
	//初期設定系統
	//メンバ関数宣言
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	Bullet* m_bullet = nullptr;
	Game::EnemyAttackPoint* m_enemyAttackPoint= nullptr;		//エネミーアタックポイント構造体のポインタ

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
	int m_hp = 4;                           //HP
	//int m_sh = 0;							//シールド
	int m_Vectornum = 0;					//配列のナンバー
	float m_idleTimer= 0.0f;								//待機時間タイマー。
	float m_chaseTimer = 0.0f;								//追跡時間タイマー。
	float m_looptimer = 0.0f;								//ループタイマー
	float looptime = 1.16;									//ループ時間
	float m_mutekitimer = 0.0f;								//無敵タイマー
	float mutekitime = 0.1f;								//無敵時間

	bool m_useAttacPoint = false;							//アタックポイントを保持しているか


	float m_receiveDamageTimer = 0.0f;	//被ダメージモーションを用意し忘れたので一時的にタイマーで管理
	EnEnemyState m_enemyOldState;          //被弾時に被ダメージステートから元のステートに戻るために、記憶しておく変数
};

